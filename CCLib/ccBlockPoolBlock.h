#pragma once

/*==============================================================================

This is a template that defines a base class that can be inherited by classes
that are members of a memory block pool, instances of the inheriting classes
use block pools for memory management.

The class template provides static member variables that instantiate and 
initialize the memory pool, and also methods that provide access to it. 
The static variables establish global variables that instantiate the
memory pools.

This is used for intrusive blocks, those that must contain a block handle 
member variable.
==============================================================================*/

#include <new>
#include <stdio.h>

#include "ccBlockPool.h"

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a class template for classes whose instances use a block pool 
// for memory management. This is used for intrusive blocks, those that must
// contain a block handle member variable.

template <int BlockPoolIndex>
class BlockPoolBlock
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Allocate a block from the block pool at the given block pool index.
   // Return a pointer to the allocated block.
   // Return null if the block pool is empty.

   static void* operator new(size_t sz)
   {

      // Block pointer.
      void* tBlockPointer = 0;

      // Try to allocate a block from the block pool.
      CC::allocateBlockPoolBlock(BlockPoolIndex, (void**)&tBlockPointer, 0);

      // Return the pointer to the allocated block.
      // Return null if the block pool is empty.
      return tBlockPointer;
   }
  

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Deallocate a block from the block pool.

   static void operator delete(void* ptr)
   {
      // Deallocate the block back to the block pool
      CC::deallocateBlockPoolBlock(ptr);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
