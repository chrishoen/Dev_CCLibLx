#pragma once

/*==============================================================================

This defines a base class for block pools. 

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "ccBlockPool.h"

namespace CC
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract base class for a pool of memory blocks. It provides a
// common interface for block pools.

class BlockPoolBase
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Abstract Methods.

   // Initialize the block pool. It is passed block pool parameters.
   virtual void initialize(BlockPoolParms* aParms)=0;

   // Deallocate memory for the block pool.
   virtual void finalize()=0;

   // Get a block from the pool, this allocates a block.
   // Return true if successful, false if the block pool is empty.
   virtual bool allocate(void** aBlockPointer,BlockHandle* aBlockHandle)=0;

   // Put a block back to the pool, this deallocates a block.
   virtual void deallocate(BlockHandle aBlockHandle)=0;

   // Get a pointer to a block, given its memory handle.
   virtual void* getBlockPtr(BlockHandle aBlockHandle)=0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Abstract Methods.

   // Size, the number of blocks that are available to be allocated.
   virtual int size()=0;

   // Show status and metrics for the block pool.
   virtual void show()=0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

