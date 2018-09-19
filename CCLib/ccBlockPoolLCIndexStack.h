#pragma once

/*==============================================================================

Free List Stack of Indices. 

This implements a free list stack of indices. It is thread safe, because
it uses a synchronization lock around critical sections.

==============================================================================*/

#include "ccBlockPoolBaseIndexStack.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CC
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a stack of indices to be used with containers that 
// implement free lists of blocks.

class BlockPoolLCIndexStack : public BlockPoolBaseIndexStack
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Class.

   // This returns the number of bytes that an instance of this class
   // will need to be allocated for it.
   static int getMemorySize(BlockPoolParms* aParms);

   class MemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If this flag is false then the memory for this object was created
   // externally. If it is true then the memory was allocated at 
   // initialization and must be freed at finalization.
   bool mOwnMemoryFlag;

   // Pointer to memory for which the stack resides. This is either created
   // externally and passed as an initialization parameter or it is created
   // on the system heap at initialization.
   void* mMemory;

   // Array of indices for the stack.
   int* mElement;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Index into the array.
   int mIndex;

   // Size of the array, number of elements allocated.
   int mNumElements;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This is a synchronization lock used to protect critical sections.
   void* mCriticalSection;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BlockPoolLCIndexStack();
  ~BlockPoolLCIndexStack();

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
   void initialize(BlockPoolParms* aParms,void* aMemory = 0);

   // Deallocate memory.
   void finalize();

   // Pop a value off of the stack. Return false if the stack is empty.
   bool pop(int* aValue);

   // Push a value onto the stack. Return false if the stack is full.
   bool push(int aValue);

   // Return size, the number of elements that have been pushed onto the stack.
   int size();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


