/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cc_functions.h"
#include "ccDefs.h"
#include "ccMemoryPtr.h"
#include "ccBlockBoxArray.h"

namespace CC
{


//******************************************************************************
//******************************************************************************
//******************************************************************************
// This local class calculates and stores the memory sizes needed by the class.

class BlockBoxArray::MemorySize
{
public:
   // Members.
   int mBlockSize;
   int mBlockBoxSize;
   int mBlockBoxArraySize;
   int mMemorySize;

   // Calculate and store memory sizes.
   MemorySize(BlockPoolParms* aParms)
   {
      mBlockSize         = cc_round_upto16(aParms->mBlockSize);
      mBlockBoxSize      = cBlockHeaderSize + mBlockSize;
      mBlockBoxArraySize = cc_round_upto16(cNewArrayExtraMemory + aParms->mNumBlocks*mBlockBoxSize);
      mMemorySize        = mBlockBoxArraySize;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This returns the number of bytes that an instance of this class
// will need to be allocated for it.

int BlockBoxArray::getMemorySize(BlockPoolParms* aParms)
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
// Constructor

BlockBoxArray::BlockBoxArray()
{
   mOwnMemoryFlag = false;
   mMemory = 0;
   mBlockBoxArray=0;

   mNumBlocks = 0;
   mBlockSize = 0;
   mBlockBoxSize = 0;
   mPoolIndex = 0;
}

BlockBoxArray::~BlockBoxArray()
{
   finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Allocate memory for the block array. It is passed the number of blocks to 
// allocate and the size of the blocks.

void BlockBoxArray::initialize(BlockPoolParms* aParms,void* aMemory)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize memory.

   // Deallocate memory, if any exists.
   finalize();

   // If the instance of this class is not to reside in external memory
   // then allocate memory for it on the system heap.
   if (aMemory == 0)
   {
      mMemory = malloc(BlockBoxArray::getMemorySize(aParms));
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
   MemorySize tMemorySize(aParms);

   // Calculate memory addresses.
   MemoryPtr tMemoryPtr(mMemory);

   char* tBlockBoxArrayMemory = tMemoryPtr.cfetch_add(tMemorySize.mBlockBoxArraySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize variables.

   // Initialize the state.
   mNumBlocks = aParms->mNumBlocks;
   mBlockSize = cc_round_upto16(aParms->mBlockSize);
   mBlockBoxSize = mBlockSize + cBlockHeaderSize;;
   mPoolIndex = aParms->mPoolIndex;

   // Set the array pointer value.
   mBlockBoxArray = tBlockBoxArrayMemory;

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

   // Store block array parameters. These can be used elsewhere.
   aParms->mBlockHeaderSize  = cBlockHeaderSize;
   aParms->mBlockBoxSize     = tMemorySize.mBlockBoxSize;
   aParms->mBlockBoxArrayPtr = MemoryPtr(mBlockBoxArray);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BlockBoxArray::finalize()
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
// Return a pointer to a block, based on block array index

char* BlockBoxArray::getBlockBoxPtr(int aIndex)
{
   char*  tBlockBox = &mBlockBoxArray[mBlockBoxSize*aIndex];
   return tBlockBox;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to a header, based on block array index

BlockHeader* BlockBoxArray::getHeaderPtr(int aIndex)
{
   char*  tBlockBox = &mBlockBoxArray[mBlockBoxSize*aIndex];
   BlockHeader* tHeader = (BlockHeader*)tBlockBox;
   return tHeader;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a pointer to a body, based on block array index

char* BlockBoxArray::getBlockPtr(int aIndex)
{
   char*  tBlockBox = &mBlockBoxArray[mBlockBoxSize*aIndex];
   char*  tBlock = tBlockBox + cBlockHeaderSize;
   return tBlock;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the handle of a block, given its address.

BlockHandle BlockBoxArray::getBlockHandle(void* aBlockPtr)
{
   BlockHandle tBlockHandle;
   if (aBlockPtr==0) return tBlockHandle;

   char*  tBlock = (char*)aBlockPtr;
   BlockHeader* tHeader = (BlockHeader*)(tBlock - cBlockHeaderSize);

   tBlockHandle = tHeader->mBlockHandle;
   return tBlockHandle;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace