#pragma once

/*==============================================================================

Free List Stack of Indices. 

This implements a free list stack of indices. It is not thread safe.

==============================================================================*/

#include "ccBlockPoolParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a stack of indices to be used with containers that 
// implement free lists of blocks.

namespace CC
{
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

class BlockPoolBaseIndexStack
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods

   // Initialize the stack to either allocate memory from the system heap or
   // to use external memory that has already been allocated for it.
   // If aMemory is null then it mallocs from the system heap. If not, then
   // it uses the memory pointed to by aMemory.
   // If external memory is used, it must be of a size obtained by a call to
   // getMemorySize.
   //
   // Initialize the stack to full. Push the indices of the blocks for which 
   // this will be used onto the stack.
   // For aAllocate==10 this will push 9,8,7,6,5,4,3,2,1,0 so that element
   // zero will be the first one poped.
   virtual void initialize(BlockPoolParms* aParms,void* aMemory = 0)=0;

   // Deallocate memory.
   virtual void finalize()=0;

   // Pop a value off of the stack. Return false if the stack is empty.
   virtual bool pop(int* aValue)=0;

   // Push a value onto the stack. Return false if the stack is full.
   virtual bool push(int aValue)=0;

   // Return size, the number of elements that have been pushed onto the stack.
   virtual int size()=0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

