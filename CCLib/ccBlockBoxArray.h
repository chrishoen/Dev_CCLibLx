#pragma once

/*==============================================================================

This defines an array of memory blocks that are dynamically allocated. It 
provides an initialization method that allocates the blocks and it provides
an indexing method that is used to access the blocks.

A block box contains a block header and a block body.

==============================================================================*/

#include "ccBlockHeader.h"
#include "ccBlockPool.h"
#include "ccBlockPoolParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsulates an array of memory blocks that are dynamically
// allocated at initialization.

class BlockBoxArray
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This returns the number of bytes that an instance of this class
   // will need to be allocated for it.

   static int getMemorySize(BlockPoolParms* aParms);

   class MemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members

   // If this flag is false then the memory for this object was created
   // externally. If it is true then the memory was allocated at 
   // initialization and must be freed at finalization.
   bool mOwnMemoryFlag;

   // Pointer to memory for which the stack resides. This is either created
   // externally and passed as an initialization parameter or it is created
   // on the system heap at initialization.
   void* mMemory;

   // Pointer to allocated memory for the block box array.
   // This is an array of bytes of size NumBlocks*BlockBoxSize.
   char* mBlockBoxArray;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Number of blocks allocated.
   int mNumBlocks;

   // Size of each block allocated.
   int mBlockSize;

   // Size of each block box allocated.
   int mBlockBoxSize;

   // Memory pool index for the block box array.
   int mPoolIndex;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods

   // Constructor
   BlockBoxArray();
  ~BlockBoxArray();

   // Initialize the array to either allocate memory from the system heap or
   // to use external memory that has already been allocated for it.
   // If aMemory is null then it mallocs from the system heap. If not, then
   // it uses the memory pointed to by aMemory.
   // If external memory is used, it must be of a size obtained by a call to
   // getMemorySize.
   // 
   // It is passed the number of blocks to allocate, the size of the block 
   // body, the memory pool index for the block box array, and an external
   // memory pointer.
   void initialize(BlockPoolParms* aParms,void* aMemory = 0);

   // Deallocate memory for the block array.
   void finalize();

   // Return a pointer to a block box, based on its block index.
   char* getBlockBoxPtr(int aIndex);

   // Return a pointer to a block header, based on its block index.
   BlockHeader* getHeaderPtr(int aIndex);

   // Return a pointer to a block body, based on its block index.
   char* getBlockPtr(int aIndex);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Get the handle of a block, given its address.

   static BlockHandle getBlockHandle(void* aBlockPtr);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

