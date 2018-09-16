#pragma once

/*==============================================================================

Lock Free Value Queue Class Template. 

It is multiple writer multiple reader thread safe.
It is lock free, non blocking.
It is shared memory safe.

This implements a value queue. The queue is thread safe. It uses a atomic
interlocked compare and exchanges to guard against concurrency contentions.
It implements the Michael and Scott algorithm with no backoff.

It is thread safe for separate multiple writer and multpile reader threads.

It implements the Michael and Scott algorithm with no backoff. It maintains
storage for the blocks by implementing a free list that uses the Trieber 
algorithm with no backoff.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <atomic>
#include <new>
#include "ccDefs.h"
#include "cc_functions.h"
#include "cc_throw.h"
#include "ccMemoryPtr.h"
#include "ccLFIndex.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CC
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// State variables for the object. These are located in a separate class
// so that they can be located in external memory.

class LFValueQueueState
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Class.

   // This returns the number of bytes that an instance of this class
   // will need to be allocated for it.
   static int getMemorySize()
   {
      return cc_round_upto16(sizeof(LFValueQueueState));
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Number of elements allocated.
   int mNumElements;
   int mQueueNumElements;
   int mListNumElements;

   // Queue variables.
   AtomicLFIndex     mQueueHead;
   AtomicLFIndex     mQueueTail;

   // Linked list variables.
   AtomicLFIndex     mListHead;
   std::atomic<int>  mListSize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   LFValueQueueState()
   {
      // All null
      mNumElements=0;
      mQueueNumElements=0;
      mListNumElements=0;
   }

   // Initialize.
   void initialize(int aNumElements,bool aConstructorFlag)
   {
      // Do not initialize, if already initialized.
      if (!aConstructorFlag) return;

      // Allocate for one extra dummy node.
      mNumElements       = aNumElements + 1;
      // Allocate for one extra dummy node.
      mQueueNumElements  = aNumElements + 1;
      // Allocate for one extra dummy node.
      mListNumElements   = aNumElements + 1;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

template <class Element>
class LFValueQueue
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Class.

   // This local class calculates and stores the memory sizes needed by the class.
   class MemorySize
   {
   public:
      // Members.
      int mStateSize;
      int mQueueArraySize;
      int mListArraySize;
      int mElementArraySize;
      int mMemorySize;

      // Calculate and store memory sizes.
      MemorySize(int aNumElements)
      {
         mStateSize         = LFValueQueueState::getMemorySize();
         mQueueArraySize    = cc_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*sizeof(AtomicLFIndex));
         mListArraySize     = cc_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*sizeof(AtomicLFIndex));
         mElementArraySize  = cc_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*sizeof(Element));
         mMemorySize = mStateSize + mQueueArraySize + mListArraySize + mElementArraySize;
      }
   };

   // This returns the number of bytes that an instance of this class
   // will need to be allocated for it.
   static int getMemorySize(int aNumElements)
   {
      MemorySize tMemorySize(aNumElements);
      return tMemorySize.mMemorySize;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If this flag is false then the memory for this object was created
   // externally. If it is true then the memory was allocated at 
   // initialization and must be freed at finalization.
   bool mOwnMemoryFlag;

   // Pointer to memory for which the queue resides. This is either created
   // externally and passed as an initialization parameter or it is created
   // on the system heap at initialization.
   void* mMemory;

   // State variables for the queue. These are located in a separate class
   // so that they can be located in externale memory.
   LFValueQueueState* mX;

   // Array of values, storage for the values.
   // Size is NumElements + 1.
   // Index range is 0..NumElements.
   Element* mElement;

   // Queue array, contains the node index of the queue next node.
   // Size is NumElements + 1.
   // Index range is 0..NumElements.
   AtomicLFIndex* mQueueNext;

   // Free List array, contains the node index of the free list next node.
   // Size is NumElements + 1.
   // Index range is 0..NumElements.
   AtomicLFIndex* mListNext;

   // Marks an invalid node.
   static const short int  cInvalid = (short int)0x8000;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   LFValueQueue()
   {
      // All null.
      mX = 0;
      mOwnMemoryFlag = false;
      mMemory = 0;

      // All null
      mElement = 0;
      mQueueNext = 0;
      mListNext = 0;
   }

   ~LFValueQueue()
   {
      finalize();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialization Methods.
   //
   // Initialization parameters:
   // Number of elements to create. There is one extra dummy element, so that
   // memory for aNumElements+1 is allocated.
   //    int aNumElements
   //
   // If true then constructors are called for all internal objects created
   // for the object. If false then the constructors are not called.
   // This is false if the object resides in shared memory and has
   // already been created. This is true otherwise.
   //    bool aConstructorFlag
   //
   // Pointer to external memory allocated for the entire object.
   // If this is null then system heap memory is allocated for the entire
   // block pool.
   //    void* aMemory
   //
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize

   void initialize(int aNumElements)
   {
      initialize(aNumElements,true,0);
   }

   void initialize(int aNumElements,bool aConstructorFlag, void* aMemory)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Initialize memory.

      // Deallocate memory, if any exists.
      finalize();

      // If the instance of this class is not to reside in external memory
      // then allocate memory for it on the system heap.
      if (aMemory == 0)
      {
         mMemory = malloc(LFValueQueue<Element>::getMemorySize(aNumElements));
         mOwnMemoryFlag = true;
      }
      // If the instance of this class is to reside in external memory
      // then use the memory pointer that was passed in.
      else
      {
         mMemory = aMemory;
         mOwnMemoryFlag = false;
      }

      // Calculate memory sizes.
      MemorySize tMemorySize(aNumElements);

      // Calculate memory addresses.
      MemoryPtr tMemoryPtr(mMemory);

      char* tStateMemory        = tMemoryPtr.cfetch_add(tMemorySize.mStateSize);
      char* tQueueArrayMemory   = tMemoryPtr.cfetch_add(tMemorySize.mQueueArraySize);
      char* tListArrayMemory    = tMemoryPtr.cfetch_add(tMemorySize.mListArraySize);
      char* tElementArrayMemory = tMemoryPtr.cfetch_add(tMemorySize.mElementArraySize);

      // Construct the state.
      if (aConstructorFlag)
      {
         // Call the constructor.
         mX = new(tStateMemory)LFValueQueueState;
      }
      else
      {
         // The constructor has already been called.
         mX = (LFValueQueueState*)tStateMemory;
      }
      // Initialize the state.
      mX->initialize(aNumElements,aConstructorFlag);

      // Construct the arrays.
      if (aConstructorFlag)
      {
         // Call the constructor.
         mQueueNext = new(tQueueArrayMemory)AtomicLFIndex[mX->mQueueNumElements];

         // Call the constructor.
         mListNext = new(tListArrayMemory)AtomicLFIndex[mX->mListNumElements];

         // Call the constructor.
         mElement = new(tElementArrayMemory)Element[mX->mNumElements];
      }
      else
      {
         // The constructor has already been called.
         mQueueNext = (AtomicLFIndex*)tQueueArrayMemory;

         // The constructor has already been called.
         mListNext = (AtomicLFIndex*)tListArrayMemory;

         // The constructor has already been called.
         mElement = (Element*)tElementArrayMemory;
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Initialize variables.

      // Initialize queue and linked list, if not already been initialized.
      if (aConstructorFlag)
      {
         // Initialize linked list array. Each node next node is the one after it.
         for (int i = 0; i < mX->mListNumElements - 1; i++)
         {
            mListNext[i].store(LFIndex(i + 1, 0));
         }
         // The last node has no next node.
         mListNext[mX->mListNumElements - 1].store(LFIndex(cInvalid, 0));

         // List head points to the first node.
         mX->mListHead.store(LFIndex(0, 0));
         // List size is initially a full free list.
         mX->mListSize = mX->mListNumElements;

         // Initialize queue array. Each node has no next node.
         for (int i = 0; i < mX->mListNumElements; i++)
         {
            mQueueNext[i].store(LFIndex(cInvalid, 0));
         }

         // Pop the dummy node.
         int tDummyNode;
         listPop(&tDummyNode);

         // initialize queue head and tail.
         mX->mQueueHead.store(LFIndex(tDummyNode, 0));
         mX->mQueueTail = mX->mQueueHead.load();
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Finalize.

   void finalize()
   {
      if (mOwnMemoryFlag)
      {
         if (mMemory)
         {
            free(mMemory);
         }
      }
      mMemory = 0;
      mOwnMemoryFlag = false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Size.

   int size()
   {
      return mX->mListNumElements - mX->mListSize.load(std::memory_order_relaxed);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This attempts to write a value to the queue. If the queue is not full
   // then it succeeds. It attempts to pop a node from the free list. If
   // the free list is empty then the queue is full and it exits. The value
   // is to be written is stored in the new node. The new node is then attached
   // to the queue tail node and the tail index is updated.

   bool tryWrite(Element aValue)
   {
      // Try to allocate a node from the free list.
      // Exit if it is empty.
      int tNodeIndex;
      if (!listPop(&tNodeIndex)) return false;

      // Initialize the node with the value.
      mElement[tNodeIndex] = aValue;
      mQueueNext[tNodeIndex].store(LFIndex(cInvalid, 0), std::memory_order_relaxed);

      // Attach the node to the queue tail.
      LFIndex tTail, tNext;

      int tLoopCount = 0;
      while (true)
      {
         tTail = mX->mQueueTail.load(std::memory_order_relaxed);
         tNext = mQueueNext[tTail.mIndex].load(std::memory_order_acquire);

         if (tTail == mX->mQueueTail.load(std::memory_order_relaxed))
         {
            if (tNext.mIndex == cInvalid)
            {
               if (mQueueNext[tTail.mIndex].compare_exchange_strong(tNext, LFIndex(tNodeIndex, tNext.mCount + 1), std::memory_order_release, std::memory_order_relaxed)) break;
            }
            else
            {
               mX->mQueueTail.compare_exchange_weak(tTail, LFIndex(tNext.mIndex, tTail.mCount + 1), std::memory_order_release, std::memory_order_relaxed);
            }
         }

         if (++tLoopCount == 10000) cc_throw(101);
      }

      mX->mQueueTail.compare_exchange_strong(tTail, LFIndex(tNodeIndex, tTail.mCount + 1), std::memory_order_release, std::memory_order_relaxed);

      // Done
      return true;
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This attempts to read a value from the queue. If the queue is not empty
   // then it succeeds. The next node in the queue to be read is the one 
   // immedialtely after the head node. It extracts the read value from the read
   // node, pushes the previous head node back onto the free list and updates the
   // head index.

   bool tryRead(Element* aValue)
   {
      LFIndex tHead, tTail, tNext;

      int tLoopCount = 0;
      while (true)
      {
         tHead = mX->mQueueHead.load(std::memory_order_relaxed);
         tTail = mX->mQueueTail.load(std::memory_order_acquire);
         tNext = mQueueNext[tHead.mIndex].load(std::memory_order_relaxed);

         if (tHead == mX->mQueueHead.load(std::memory_order_acquire))
         {
            if (tHead.mIndex == tTail.mIndex)
            {
               if (tNext.mIndex == cInvalid) return false;
               mX->mQueueTail.compare_exchange_strong(tTail, LFIndex(tNext.mIndex, tTail.mCount + 1), std::memory_order_release, std::memory_order_relaxed);
            }
            else
            {
               *aValue = mElement[tNext.mIndex];
               if (mX->mQueueHead.compare_exchange_strong(tHead, LFIndex(tNext.mIndex, tHead.mCount + 1), std::memory_order_acquire, std::memory_order_relaxed))break;
            }
         }

         if (++tLoopCount == 10000) cc_throw(102);
      }

      listPush(tHead.mIndex);

      // Done.
      return true;
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This detaches the head node.

   bool listPop(int* aNode)
   {
      // Store the head node in a temp.
      // This is the node that will be detached.
      LFIndex tHead = mX->mListHead.load(std::memory_order_relaxed);

      int tLoopCount = 0;
      while (true)
      {
         // Exit if the list is empty.
         if (tHead.mIndex == cInvalid) return false;

         // Set the head node to be the node that is after the head node.
         if (mX->mListHead.compare_exchange_weak(tHead, LFIndex(mListNext[tHead.mIndex].load(std::memory_order_relaxed).mIndex, tHead.mCount + 1), std::memory_order_acquire, std::memory_order_relaxed)) break;

         if (++tLoopCount == 10000) cc_throw(103);
      }

      // Return the detached original head node.
      *aNode = tHead.mIndex;

      // Done.
      mX->mListSize.fetch_sub(1, std::memory_order_relaxed);
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Insert a node into the list before the list head node.

   bool listPush(int aNode)
   {
      // Store the head node in a temp.
      LFIndex tHead = mX->mListHead.load(std::memory_order_relaxed);

      int tLoopCount = 0;
      while (true)
      {
         // Attach the head node to the pushed node.
         mListNext[aNode].store(tHead, std::memory_order_relaxed);

         // The pushed node is the new head node.
         std::atomic<short int>* tListHeadIndexPtr = (std::atomic<short int>*)&mX->mListHead;
         if ((*tListHeadIndexPtr).compare_exchange_weak(tHead.mIndex, aNode, std::memory_order_release, std::memory_order_relaxed)) break;
         if (++tLoopCount == 10000) cc_throw(103);
      }

      // Done.
      mX->mListSize.fetch_add(1, std::memory_order_relaxed);
      return true;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

