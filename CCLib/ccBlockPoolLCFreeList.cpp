/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cc_functions.h"
#include "ccMemoryPtr.h"
#include "ccBlockPoolLCIndexStack.h"
#include "ccBlockPoolLCFreeList.h"

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This local class calculates and stores the memory sizes needed by the class.

class BlockPoolLCFreeList::MemorySize
{
public:
   // Members.
   int mBlockBoxArraySize;
   int mStackSize;
   int mMemorySize;

   // Calculate and store memory sizes.
   MemorySize(BlockPoolParms* aParms)
   {
      mBlockBoxArraySize = BlockBoxArray::getMemorySize(aParms);
      mStackSize = BlockPoolLCIndexStack::getMemorySize(aParms);
      mMemorySize = mBlockBoxArraySize + mStackSize;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This returns the number of bytes that an instance of this class
// will need to be allocated for it.

int BlockPoolLCFreeList::getMemorySize(BlockPoolParms* aParms)
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

BlockPoolLCFreeList::BlockPoolLCFreeList()
{
   // All null.
   mOwnMemoryFlag = false;
   mMemory = 0;
   mBlockIndexStack = 0;
}

BlockPoolLCFreeList::~BlockPoolLCFreeList()
{
   finalize();
}

//******************************************************************************
//******************************************************************************
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

void BlockPoolLCFreeList::initialize(BlockPoolParms* aParms)
{
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Initialize memory.

   // Deallocate memory, if any exists.
   finalize();

   // Create the index stack.
   mBlockIndexStack = new BlockPoolLCIndexStack;

   // If the instance of this class is not to reside in external memory
   // then allocate memory for it on the system heap.
   if (aParms->mMemory == 0)
   {
      mMemory = malloc(BlockPoolLCFreeList::getMemorySize(aParms));
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
   char* tStackMemory         = tMemoryPtr.cfetch_add(tMemorySize.mStackSize);

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Initialize variables.

   // Store the pointer to the parameters.
   mParms = aParms;

   // Initialize the block box array.
   mBlocks.initialize(aParms,tBlockBoxArrayMemory);

   // Initialize the index stack.
   mBlockIndexStack->initialize(aParms,tStackMemory);

   // Mark this block pool initialization as valid.
   aParms->mValidFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Deallocate memory for the block pool.

void BlockPoolLCFreeList::finalize()
{
   mBlocks.finalize();

   if (mBlockIndexStack)
   {
      mBlockIndexStack->finalize();
      delete mBlockIndexStack;
      mBlockIndexStack = 0;
   }

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

int BlockPoolLCFreeList::size()
{ 
   if (mBlockIndexStack==0) return 0;
   return mBlockIndexStack->size();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get a block from the pool, this allocates a block.
// Return true if successful, false if the block pool is empty.

bool BlockPoolLCFreeList::allocate(void** aBlockPointer,BlockHandle* aBlockHandle)
{
   int tBlockIndex = 0;
      
   // Try to pop a block index from the index stack, as a free list.
   // If the stack is not empty
   if (mBlockIndexStack->pop(&tBlockIndex))
   {
      // Return a pointer to the block at that index.
      if (aBlockPointer)
      {
         *aBlockPointer = mBlocks.getBlockPtr(tBlockIndex);
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

void BlockPoolLCFreeList::deallocate(BlockHandle aBlockHandle)
{
   // Push the block index back onto the stack
   mBlockIndexStack->push(aBlockHandle.mBlockIndex);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to a block, given its memory handle.

void* BlockPoolLCFreeList::getBlockPtr(BlockHandle aBlockHandle)
{
   // Return the address of the block within the block array.
   return mBlocks.getBlockPtr(aBlockHandle.mBlockIndex);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Helpers.

void BlockPoolLCFreeList::show()
{
   printf("BlockPoolLCFreeList size %d $ %d\n", mParms->mPoolIndex,size());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace