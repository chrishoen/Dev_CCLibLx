/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cc_functions.h"
#include "cc_throw.h"
#include "ccDefs.h"
#include "ccMemoryPtr.h"
#include "ccLFObjectQueue.h"

using namespace std;

namespace CC
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor, initialize members for an empty stack of size zero. 

int LFObjectQueueState::getMemorySize()
{
   return cc_round_upto16(sizeof(LFObjectQueueState));
}

LFObjectQueueState::LFObjectQueueState()
{
   // All null
   mNumElements=0;
   mQueueNumElements=0;
   mListNumElements=0;
   mElementSize=0;
}

void LFObjectQueueState::initialize(int aNumElements,int aElementSize,bool aConstructorFlag)
{
   // Do not initialize, if already initialized.
   if (!aConstructorFlag) return;

   // Allocate for one extra dummy node.
   mNumElements       = aNumElements + 1;
   // Allocate for one extra dummy node.
   mQueueNumElements  = aNumElements + 1;
   // Allocate for one extra dummy node.
   mListNumElements   = aNumElements + 1;
   // Store.
   mElementSize       = aElementSize;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// This local class calculates and stores the memory sizes needed by the class.

class LFObjectQueue::MemorySize
{
public:
   // Members.
   int mStateSize;
   int mQueueArraySize;
   int mListArraySize;
   int mElementArraySize;
   int mMemorySize;

   // Calculate and store memory sizes.
   MemorySize(int aNumElements,int aElementSize)
   {
      mStateSize         = LFObjectQueueState::getMemorySize();
      mQueueArraySize    = cc_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*sizeof(AtomicLFIndex));
      mListArraySize     = cc_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*sizeof(AtomicLFIndex));
      mElementArraySize  = cc_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*aElementSize);
      mMemorySize = mStateSize + mQueueArraySize + mListArraySize + mElementArraySize;
   }
};

//***************************************************************************
//***************************************************************************
//***************************************************************************
// This returns the number of bytes that an instance of this class
// will need to be allocated for it.

int LFObjectQueue::getMemorySize(int aNumElements,int aElementSize)
{
   MemorySize tMemorySize(aNumElements,aElementSize);
   return tMemorySize.mMemorySize;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

LFObjectQueue::LFObjectQueue()
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

LFObjectQueue::~LFObjectQueue()
{
   finalize();
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Initialize

void LFObjectQueue::initialize(int aNumElements,int aElementSize)
{
   initialize(aNumElements,aElementSize,true,0);
}

void LFObjectQueue::initialize(int aNumElements,int aElementSize,bool aConstructorFlag, void* aMemory)
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
      mMemory = malloc(LFObjectQueue::getMemorySize(aNumElements,aElementSize));
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
   MemorySize tMemorySize(aNumElements,aElementSize);

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
      mX = new(tStateMemory)LFObjectQueueState;
   }
   else
   {
      // The constructor has already been called.
      mX = (LFObjectQueueState*)tStateMemory;
   }
   // Initialize the state.
   mX->initialize(aNumElements,aElementSize,aConstructorFlag);

   // Construct the arrays.
   if (aConstructorFlag)
   {
      // Call the constructor.
      mQueueNext = new(tQueueArrayMemory)AtomicLFIndex[mX->mQueueNumElements];

      // Call the constructor.
      mListNext = new(tListArrayMemory)AtomicLFIndex[mX->mListNumElements];
   }
   else
   {
      // The constructor has already been called.
      mQueueNext = (AtomicLFIndex*)tQueueArrayMemory;

      // The constructor has already been called.
      mListNext = (AtomicLFIndex*)tListArrayMemory;

   }

   // There is no constructor for this array.
   mElement = tElementArrayMemory;

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
      // List size is initially a full stack.
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

void LFObjectQueue::finalize()
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

int LFObjectQueue::size()
{
   return mX->mListNumElements - mX->mListSize.load(std::memory_order_relaxed);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to an object, based on its object index.

void* LFObjectQueue::elementAt(int aIndex)
{
   return (void*)((char*)mElement + mX->mElementSize*aIndex);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// This attempts to write a value to the queue. If the queue is not full
// then it succeeds. It attempts to pop a node from the free list. If
// the free list is empty then the queue is full and it exits. The value
// is to be written is stored in the new node. The new node is then attached
// to the queue tail node and the tail index is updated.

void* LFObjectQueue::startWrite(int* aNodeIndex)
{
   // Try to allocate a node from the free list.
   // Exit if it is empty.
   int tNodeIndex;
   if (!listPop(&tNodeIndex)) return 0;

   // Return a pointer to the node element.
   *aNodeIndex = tNodeIndex;
   return elementAt(tNodeIndex);
}

void LFObjectQueue::finishWrite(int aNodeIndex)
{
   // Initialize the node.
   int tNodeIndex = aNodeIndex;
   mQueueNext[tNodeIndex].store(LFIndex(cInvalid, 0), memory_order_relaxed);

   // Attach the node to the queue tail.
   LFIndex tTail,tNext;

   int tLoopCount=0;
   while (true)
   {
      tTail = mX->mQueueTail.load(memory_order_relaxed);
      tNext = mQueueNext[tTail.mIndex].load(memory_order_acquire);

      if (tTail == mX->mQueueTail.load(memory_order_relaxed))
      {
         if (tNext.mIndex == cInvalid)
         {
            if (mQueueNext[tTail.mIndex].compare_exchange_strong(tNext, LFIndex(tNodeIndex, tNext.mCount+1),memory_order_release,memory_order_relaxed)) break;
         }
         else
         {
            mX->mQueueTail.compare_exchange_weak(tTail, LFIndex(tNext.mIndex, tTail.mCount+1),memory_order_release,memory_order_relaxed);
         }
      }

      if (++tLoopCount==10000) cc_throw(101);
   }

   mX->mQueueTail.compare_exchange_strong(tTail, LFIndex(tNodeIndex, tTail.mCount+1),memory_order_release,memory_order_relaxed);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This attempts to read a value from the queue. If the queue is not empty
// then it succeeds. The next node in the queue to be read is the one 
// immedialtely after the head node. It extracts the read value from the read
// node, pushes the previous head node back onto the free list and updates the
// head index.

void* LFObjectQueue::startRead(int* aNodeIndex)
{
   void* tElementPtr = 0;
   LFIndex tHead, tTail, tNext;

   int tLoopCount=0;
   while (true)
   {
      tHead = mX->mQueueHead.load(memory_order_relaxed);
      tTail = mX->mQueueTail.load(memory_order_acquire);
      tNext = mQueueNext[tHead.mIndex].load(memory_order_relaxed);

      if (tHead == mX->mQueueHead.load(memory_order_acquire))
      {
         if (tHead.mIndex == tTail.mIndex)
         {
            if (tNext.mIndex == cInvalid) return 0;
            mX->mQueueTail.compare_exchange_strong(tTail, LFIndex(tNext.mIndex, tTail.mCount+1),memory_order_release,memory_order_relaxed);
         }
         else
         {
            tElementPtr = elementAt(tNext.mIndex);
            if (mX->mQueueHead.compare_exchange_strong(tHead, LFIndex(tNext.mIndex, tHead.mCount+1),memory_order_acquire,memory_order_relaxed))break;
         }
      }

      if (++tLoopCount==10000) cc_throw(102);
   }

   *aNodeIndex = tHead.mIndex;

   // Done.
   return tElementPtr;
}

void LFObjectQueue::finishRead(int aNodeIndex)
{
   listPush(aNodeIndex);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This detaches the head node.

bool LFObjectQueue::listPop(int* aNode)
{
   // Store the head node in a temp.
   // This is the node that will be detached.
   LFIndex tHead = mX->mListHead.load(memory_order_relaxed);

   int tLoopCount=0;
   while (true)
   {
      // Exit if the list is empty.
      if (tHead.mIndex == cInvalid) return false;

      // Set the head node to be the node that is after the head node.
      if (mX->mListHead.compare_exchange_weak(tHead, LFIndex(mListNext[tHead.mIndex].load(memory_order_relaxed).mIndex,tHead.mCount+1),memory_order_acquire,memory_order_relaxed)) break;

      if (++tLoopCount==10000) cc_throw(103);
   }

   // Return the detached original head node.
   *aNode = tHead.mIndex;

   // Done.
   mX->mListSize.fetch_sub(1,memory_order_relaxed);
   return true;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Insert a node into the list before the list head node.

bool LFObjectQueue::listPush(int aNode)
{
   // Store the head node in a temp.
   LFIndex tHead = mX->mListHead.load(memory_order_relaxed);

   int tLoopCount=0;
   while (true)
   {
      // Attach the head node to the pushed node.
      mListNext[aNode].store(tHead,memory_order_relaxed);

      // The pushed node is the new head node.
      atomic<short int>* tListHeadIndexPtr = (std::atomic<short int>*)&mX->mListHead;
      if ((*tListHeadIndexPtr).compare_exchange_weak(tHead.mIndex, aNode,memory_order_release,memory_order_relaxed)) break;
      if (++tLoopCount == 10000) cc_throw(103);
   }

   // Done.
   mX->mListSize.fetch_add(1,memory_order_relaxed);
   return true;
}
//***************************************************************************
//***************************************************************************
//***************************************************************************
}//namespace