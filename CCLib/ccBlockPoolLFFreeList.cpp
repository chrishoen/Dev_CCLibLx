/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cc_functions.h"
#include "cc_throw.h"
#include "ccMemoryPtr.h"
#include "ccBlockPoolLFFreeList.h"

using namespace std;

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This local class calculates and stores the memory sizes needed by the class.

class BlockPoolLFFreeList::MemorySize
{
public:
   // Members.
   int mBlockSize;
   int mBlockBoxSize;
   int mBlockBoxArraySize;
   int mFreeListNextSize;
   int mMemorySize;

   // Calculate and store memory sizes.
   MemorySize(BlockPoolParms* aParms)
   {
      mBlockSize         = cc_round_upto16(aParms->mBlockSize);
      mBlockBoxSize      = cBlockHeaderSize + mBlockSize;
      mBlockBoxArraySize = cc_round_upto16(cNewArrayExtraMemory + aParms->mNumBlocks*mBlockBoxSize);
      mFreeListNextSize  = cc_round_upto16(cNewArrayExtraMemory + (aParms->mNumBlocks + 1)*sizeof(AtomicLFIndex));

      mMemorySize = mBlockBoxArraySize + mFreeListNextSize;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This returns the number of bytes that an instance of this class
// will need to be allocated for it.

int BlockPoolLFFreeList::getMemorySize(BlockPoolParms* aParms)
{
   MemorySize tMemorySize(aParms);
   return tMemorySize.mMemorySize;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

BlockPoolLFFreeList::BlockPoolLFFreeList()
{
   mOwnMemoryFlag = false;
   mMemory = 0;
   mFreeListNext = 0;
   mBlockBoxArray=0;

   mNumBlocks = 0;
   mBlockSize = 0;
   mBlockBoxSize = 0;
   mPoolIndex = 0;
   mFreeListNumNodes = 0;
   mFreeListSize = 0;
}

BlockPoolLFFreeList::~BlockPoolLFFreeList()
{
   finalize();
}

//******************************************************************************
;//******************************************************************************
//******************************************************************************
// This initializes the block pool. It allocates memory for the block array
// and initializes the index stack. It is passed the number of blocks to
// allocate and the size of the blocks.
//
// For aNumBlocks==10 blocks will range 0,1,2,3,4,5,6,7,8,9
// A block index of cInvalid indicates an invalid block.
//
// An index stack is used to manage free list access to the blocks
// The stack is initialized for a free list by pushing indices onto it.
// For aAllocate==10 this will push 9,7,8,6,5,4,3,2,1,0
//
// When a block is allocated, an index is popped off of the stack.
// When a block is deallocated, its index is pushed back onto the stack.
//

void BlockPoolLFFreeList::initialize(BlockPoolParms* aParms)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize memory.

   // Deallocate memory, if any exists.
   finalize();

   // If the instance of this class is not to reside in external memory
   // then allocate memory for it on the system heap.
   if (aParms->mMemory == 0)
   {
      mMemory = malloc(BlockPoolLFFreeList::getMemorySize(aParms));
      mOwnMemoryFlag = true;
   }
   // If the instance of this class is to reside in external memory
   // then use the memory pointer that was passed in.
   else
   {
      mMemory = aParms->mMemory;
      mOwnMemoryFlag = false;
   }

   // Calculate memory sizes.
   MemorySize tMemorySize(aParms);

   // Calculate memory addresses.
   MemoryPtr tMemoryPtr(mMemory);

   char* tBlockBoxArrayMemory = tMemoryPtr.cfetch_add(tMemorySize.mBlockBoxArraySize);
   char* tFreeListNextMemory  = tMemoryPtr.cfetch_add(tMemorySize.mFreeListNextSize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize state.

   // Store members.
   mNumBlocks = aParms->mNumBlocks;
   mBlockSize = cc_round_upto16(aParms->mBlockSize);
   mBlockBoxSize = cBlockHeaderSize + mBlockSize;
   mPoolIndex = aParms->mPoolIndex;

   // Allocate for one extra dummy node.
   mFreeListNumNodes = aParms->mNumBlocks + 1;
   mFreeListSize = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize block box array.

   // Set the array pointer value.
   mBlockBoxArray = tBlockBoxArrayMemory;
   mBlockBoxArrayPtr.set(tBlockBoxArrayMemory);

   // Initialize block headers.
   for (int i = 0; i < mNumBlocks; i++)
   {
      // Header pointer.
      BlockHeader* tHeader = getHeaderPtr(i);
      // Call Header constructor.
      new(tHeader)BlockHeader;
      // Set header variables.
      tHeader->mBlockHandle.set(mPoolIndex, i);
   }

   // Construct the linked list array.
   mFreeListNext = new(tFreeListNextMemory)AtomicLFIndex[mFreeListNumNodes];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize free list.

   // Initialize linked list array. Each node next node is the one after it.
   for (int i = 0; i < mFreeListNumNodes - 1; i++)
   {
      mFreeListNext[i].store(LFIndex(i + 1, 0));
   }
   // The last node has no next node.
   mFreeListNext[mFreeListNumNodes - 1].store(LFIndex(cInvalid, 0));

   // List head points to the first node.
   mFreeListHead.store(LFIndex(0, 0));
   // List size is initially a full stack.
   mFreeListSize = mFreeListNumNodes;

   // Pop the dummy node.
   int tDummyNode;
   listPop(&tDummyNode);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize parameters.

   // Store the pointer to the parameters.
   mParms = aParms;

   // Mark this block pool initialization as valid.
   aParms->mValidFlag = true;

   // Store block array parameters. These can be used elsewhere.
   aParms->mBlockHeaderSize  = cBlockHeaderSize;
   aParms->mBlockBoxSize     = tMemorySize.mBlockBoxSize;
   aParms->mBlockBoxArrayPtr = mBlockBoxArrayPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Deallocate memory for the block pool.

void BlockPoolLFFreeList::finalize()
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

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Size, the number of blocks that are available to be allocated.

int BlockPoolLFFreeList::size()
{ 
   return mFreeListSize.load(memory_order_relaxed);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get a block from the pool, this allocates a block.
// Return true if successful, false if the block pool is empty.

bool BlockPoolLFFreeList::allocate(void** aBlockPointer,BlockHandle* aBlockHandle)
{
   int tBlockIndex = 0;
      
   // Try to pop a block index from the index stack, as a free list.
   // If the stack is not empty
   if (listPop(&tBlockIndex))
   {
      // Return a pointer to the block at that index.
      if (aBlockPointer)
      {
         *aBlockPointer = mBlockBoxArrayPtr.vplus(mBlockBoxSize*tBlockIndex + cBlockHeaderSize);
#if 0
         *aBlockPointer = getBlockPtr(tBlockIndex);
#endif
      }

      // Return the memory handle for the block.
      if (aBlockHandle)
      {
         aBlockHandle->set(mParms->mPoolIndex, tBlockIndex);
      }
      return true;
   }
   // If the stack is empty
   else
   {
      // Return a null pointer.
      if (aBlockPointer)
      {
         *aBlockPointer = 0;
      }

      // Return a null memory handle.
      if (aBlockHandle)
      {
         aBlockHandle->setNull();
      }
      return false;
   }
   // Done
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Put a block back to the pool, this deallocates a block.

void BlockPoolLFFreeList::deallocate(BlockHandle aBlockHandle)
{
   // Push the block index back onto the stack
   listPush(aBlockHandle.mBlockIndex);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to a block, given its memory handle.

void* BlockPoolLFFreeList::getBlockPtr(BlockHandle aBlockHandle)
{
   return mBlockBoxArrayPtr.vplus(mBlockBoxSize*aBlockHandle.mBlockIndex + cBlockHeaderSize);
#if 0
   // Return the address of the block within the block array.
   return getBlockPtr(aBlockHandle.mBlockIndex);
#endif
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to a block, based on block array index

char* BlockPoolLFFreeList::getBlockBoxPtr(int aBlockIndex)
{
   return mBlockBoxArrayPtr.cplus(mBlockBoxSize*aBlockIndex + cBlockHeaderSize);
#if 0
   char*  tBlockBox = &mBlockBoxArray[mBlockBoxSize*aIndex];
   return tBlockBox;
#endif
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to a header, based on block array index

BlockHeader* BlockPoolLFFreeList::getHeaderPtr(int aIndex)
{
   char* tBlockBox = mBlockBoxArrayPtr.cplus(mBlockBoxSize*aIndex);
   BlockHeader* tHeader = (BlockHeader*)tBlockBox;
   return tHeader;
#if 0
   char* tBlockBox = &mBlockBoxArray[mBlockBoxSize*aIndex];
   BlockHeader* tHeader = (BlockHeader*)tBlockBox;
   return tHeader;
#endif
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to a body, based on block array index

char* BlockPoolLFFreeList::getBlockPtr(int aBlockIndex)
{
   return mBlockBoxArrayPtr.cplus(mBlockBoxSize*aBlockIndex + cBlockHeaderSize);

#if 0
   char*  tBlockBox = &mBlockBoxArray[mBlockBoxSize*aIndex];
   char*  tBlock = tBlockBox + cBlockHeaderSize;
   return tBlock;
#endif
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the handle of a block, given its address.

BlockHandle BlockPoolLFFreeList::getBlockHandle(void* aBlockPointer)
{
   BlockHandle tBlockHandle;
   if (aBlockPointer==0) return tBlockHandle;

   MemoryPtr tBlockPtr(aBlockPointer);
   BlockHeader* tHeader = (BlockHeader*)tBlockPtr.vminus(cBlockHeaderSize);
   tBlockHandle = tHeader->mBlockHandle;
   return tBlockHandle;

#if 0
   BlockHandle tBlockHandle;
   if (aBlockPtr==0) return tBlockHandle;

   char*  tBlock = (char*)aBlockPtr;
   BlockHeader* tHeader = (BlockHeader*)(tBlock - cBlockHeaderSize);

   tBlockHandle = tHeader->mBlockHandle;
   return tBlockHandle;
#endif

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Helpers

void BlockPoolLFFreeList::show()
{
   printf("BlockPoolLFFreeList size %d $ %d\n", mParms->mPoolIndex,size());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This detaches the head node.
// Use an offset of one so that pop and push indices range 0..NumElements-1.

bool BlockPoolLFFreeList::listPop(int* aNodeIndex)
{
   // Store the head node in a temp.
   // This is the node that will be detached.
   LFIndex tHead = mFreeListHead.load(memory_order_relaxed);

   int tLoopCount=0;
   while (true)
   {
      // Exit if the list is empty.
      if (tHead.mIndex == cInvalid) return false;

      // Set the head node to be the node that is after the head node.
      if (mFreeListHead.compare_exchange_weak(tHead, LFIndex(mFreeListNext[tHead.mIndex].load(memory_order_relaxed).mIndex,tHead.mCount+1),memory_order_acquire,memory_order_relaxed)) break;

      if (++tLoopCount==10000) cc_throw(103);
   }
   mFreeListSize.fetch_sub(1,memory_order_relaxed);

   // Return the detached original head node.
   *aNodeIndex = tHead.mIndex - 1;
   return true;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Insert a node into the list before the list head node.
// Use an offset of one so that pop and push indices range 0..NumElements-1.

bool BlockPoolLFFreeList::listPush(int aNodeIndex)
{
   int tNodeIndex = aNodeIndex + 1;

   // Store the head node in a temp.
   LFIndex tHead = mFreeListHead.load(memory_order_relaxed);

   int tLoopCount=0;
   while (true)
   {
      // Attach the head node to the pushed node.
      mFreeListNext[tNodeIndex].store(tHead,memory_order_relaxed);

      // The pushed node is the new head node.
      atomic<short int>* tListHeadIndexPtr = (std::atomic<short int>*)&mFreeListHead;
      if ((*tListHeadIndexPtr).compare_exchange_weak(tHead.mIndex, tNodeIndex,memory_order_release,memory_order_relaxed)) break;
      if (++tLoopCount == 10000) cc_throw(103);
   }

   // Done.
   mFreeListSize.fetch_add(1,memory_order_relaxed);
   return true;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
}//namespace