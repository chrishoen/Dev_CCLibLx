#include "stdafx.h"

#include <atomic>

#include "LFIndex.h"
#include "LFBackoff.h"
#include "LFFreeList.h"

using namespace std;

namespace LFFreeList
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Support

   static const short int cInvalid = (short int)0x80000000;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Node members

   static AtomicLFIndex* mListNext  = 0;
   static AtomicLFIndex  mStub;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Free List Members

   bool listPop (int* aNode);
   bool listPush (int aNode);

   atomic<short int>   mListSize;
   AtomicLFIndex mListHead;
   
   atomic<short int>* mListHeadIndexPtr = (atomic<short int>*)&mListHead;
   atomic<short int>& mListHeadIndexRef = (atomic<short int>&)*mListHeadIndexPtr;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Memory Members

   // Number of blocks allocated
   static int mListAllocate = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Backoff Members

   static int mBackoffList1;
   static int mBackoffList2;

   void setBackoff(double aList1, double aList2)
   {
      mBackoffList1  = LFBackoff::convertFromUsec(aList1);
      mBackoffList2  = LFBackoff::convertFromUsec(aList2);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Metrics Members

   atomic<unsigned long long> mPopRetry;
   atomic<unsigned long long> mPopRetry1;
   atomic<unsigned long long> mPopRetry2;
   atomic<unsigned long long> mPopRetry3;

   atomic<unsigned long long> mPushRetry;
   atomic<unsigned long long> mPushRetry1;
   atomic<unsigned long long> mPushRetry2;
   atomic<unsigned long long> mPushRetry3;

   unsigned long long popRetry()   {return mPopRetry.load();}
   unsigned long long pushRetry()  {return mPushRetry.load();}

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize

   void initialize(int aAllocate)
   {
      finalize();

      mListAllocate  = aAllocate;

      mListNext = new AtomicLFIndex[mListAllocate];

      for (int i = 0; i < mListAllocate - 1; i++)
      {
         mListNext[i].store(LFIndex(i+1,0));
      }

      mListNext[mListAllocate-1].store(LFIndex(cInvalid,0));

      mListHead.store(LFIndex(0,0));
      mListSize = mListAllocate;

      mPopRetry   = 0;
      mPopRetry1   = 0;
      mPopRetry2   = 0;
      mPopRetry3   = 0;
      mPushRetry  = 0;
      mPushRetry1  = 0;
      mPushRetry2  = 0;
      mPushRetry3  = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Finalize

   void finalize()
   {
      if (mListNext)
      {
         free(mListNext);
         mListNext = 0;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show

   int listSize(){ return mListSize; }

   void show()
   {
      char tString[40];
      Prn::print(0,"LFFreeList---------------------------\n");
      Prn::print(0,"PopRetry           %16s",my_stringLLU(tString,mPopRetry));
      Prn::print(0,"PopRetry1          %16s",my_stringLLU(tString,mPopRetry1));
      Prn::print(0,"PopRetry2          %16s",my_stringLLU(tString,mPopRetry2));
      Prn::print(0,"PopRetry3          %16s",my_stringLLU(tString,mPopRetry3));
      Prn::print(0,"");
      Prn::print(0,"PushRetry          %16s",my_stringLLU(tString,mPushRetry));
      Prn::print(0,"PushRetry1         %16s",my_stringLLU(tString,mPushRetry1));
      Prn::print(0,"PushRetry2         %16s",my_stringLLU(tString,mPushRetry2));
      Prn::print(0,"PushRetry3         %16s",my_stringLLU(tString,mPushRetry3));
      Prn::print(0,"");
   }


   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This detaches the head node.

   bool listPop(int* aNode)
   {
      // Store the head node in a temp.
      // This is the node that will be detached.
      LFIndex tHead = mListHead.load(memory_order_relaxed);

      int tLoopCount=0;
      while (true)
      {
         // Exit if the list is empty.
         if (tHead.mIndex == cInvalid) return false;

         // Set the head node to be the node that is after the head node.
         if (mListHead.compare_exchange_weak(tHead, LFIndex(mListNext[tHead.mIndex].load(memory_order_relaxed).mIndex,tHead.mCount+1),memory_order_acquire,memory_order_relaxed)) break;

         if (++tLoopCount==10000) throw 103;
      }
      if (tLoopCount != 0)
      {
         mPopRetry.fetch_add(1,memory_order_relaxed);
         if (tLoopCount == 1) mPopRetry1.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 2) mPopRetry2.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 3) mPopRetry3.fetch_add(1,memory_order_relaxed);
      }

      // Return the detached original head node.
      *aNode = tHead.mIndex;

      // Done.
      mListSize.fetch_sub(1,memory_order_relaxed);
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Insert a node into the list before the list head node.

   bool listPush(int aNode)
   {
      // Store the head node in a temp.
      LFIndex tHead = mListHead.load(memory_order_relaxed);

      int tLoopCount=0;
      while (true)
      {
         // Attach the head node to the pushed node.
         mListNext[aNode].store(tHead,memory_order_relaxed);

         // The pushed node is the new head node.
         if (mListHeadIndexRef.compare_exchange_weak(tHead.mIndex, aNode,memory_order_release,memory_order_relaxed)) break;
         if (++tLoopCount == 10000) throw 103;
      }
      if (tLoopCount != 0)
      {
         mPushRetry.fetch_add(1,memory_order_relaxed);
         if (tLoopCount == 1) mPushRetry1.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 2) mPushRetry2.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 3) mPushRetry3.fetch_add(1,memory_order_relaxed);
      }

      // Done.
      mListSize.fetch_add(1,memory_order_relaxed);
      return true;
   }

   
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Stub used for timing tests.

   void listStub()
   {
      LFIndex tStub;

      LFBackoff tBackoff(mBackoffList1,mBackoffList2);
      int tLoopCount=0;
      while (true)
      {
         // Store the stub in a temp.
         tStub = mStub.load();

         // Increment the modification counter.
         if (mStub.compare_exchange_weak(tStub, LFIndex(tStub.mIndex,tStub.mCount+1))) break;

         if (++tLoopCount==10000) throw 103;
         tBackoff.expBackoff();
      }
      if (tLoopCount != 0)
      {
         mPopRetry.fetch_add(1,memory_order_relaxed);
         if (tLoopCount == 1) mPopRetry1.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 2) mPopRetry2.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 3) mPopRetry3.fetch_add(1,memory_order_relaxed);
      }
   }
}//namespace

/*==============================================================================

Non-blocking stack [Treiber’s algorithm]

proc push(new)
do
  old = top
  new.next = old
while not CAS(top, old, new)
end

proc pop
do
  old = top
  return null if old == null
  new = old.next
while not CAS(top, old, new)
return old
end

==============================================================================*/



#if 0
   bool listPop(int* aNode)
   {
      LFIndex tHead;

      LFBackoff tBackoff(mBackoffList1,mBackoffList2);
      int tLoopCount=0;
      while (true)
      {
         // Store the head node in a temp.
         // This is the node that will be detached.
         tHead = mListHead.load();

         // Exit if the list is empty.
         if (tHead.mIndex == cInvalid) return false;

         // Set the head node to be the node that is after the head node.
         if (mListHead.compare_exchange_weak(tHead, LFIndex(mListNext[tHead.mIndex].load().mIndex,tHead.mCount+1))) break;

         if (++tLoopCount==10000) throw 103;
         tBackoff.expBackoff();
      }
      if (tLoopCount != 0)
      {
         mPopRetry.fetch_add(1,memory_order_relaxed);
         if (tLoopCount == 1) mPopRetry1.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 2) mPopRetry2.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 3) mPopRetry3.fetch_add(1,memory_order_relaxed);
      }

      // Return the detached original head node.
      *aNode = tHead.mIndex;

      // Done.
      mListSize.fetch_sub(1,memory_order_relaxed);
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Insert a node into the list before the list head node.

   bool listPush(int aNode)
   {
      LFIndex tHead;

      LFBackoff tBackoff(mBackoffList1,mBackoffList2);
      int tLoopCount=0;
      while (true)
      {
         // Store the head node in a temp.
         tHead = mListHead.load();

         // Attach the head node to the pushed node .
         mListNext[aNode].store(tHead);

         // The pushed node is the new head node.
         if (mListHeadIndexRef.compare_exchange_weak(tHead.mIndex, aNode)) break;
         if (++tLoopCount == 10000) throw 103;
         tBackoff.expBackoff();
      }
      if (tLoopCount != 0)
      {
         mPushRetry.fetch_add(1,memory_order_relaxed);
         if (tLoopCount == 1) mPushRetry1.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 2) mPushRetry2.fetch_add(1,memory_order_relaxed);
         else if (tLoopCount == 3) mPushRetry3.fetch_add(1,memory_order_relaxed);
      }

      // Done.
      mListSize.fetch_add(1,memory_order_relaxed);
      return true;
   }

#endif