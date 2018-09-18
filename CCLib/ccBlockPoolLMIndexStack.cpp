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
#include "ccBlockPoolLMIndexStack.h"

using namespace std;

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This local class calculates and stores the memory sizes needed by the class.

class BlockPoolLMIndexStack::MemorySize
{
public:
   // Members.
   int mArraySize;
   int mMemorySize;

   // Calculate and store memory sizes.
   MemorySize(BlockPoolParms* aParms)
   {
      mArraySize = cc_round_upto16(cNewArrayExtraMemory + aParms->mNumBlocks*sizeof(int));
      mMemorySize = mArraySize;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This returns the number of bytes that an instance of this class
// will need to be allocated for it.

int BlockPoolLMIndexStack::getMemorySize(BlockPoolParms* aParms)
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
// Constructor, initialize members for an empty stack of size zero 

BlockPoolLMIndexStack::BlockPoolLMIndexStack()
{
   mElement = 0;
   mOwnMemoryFlag = false;
   mMemory = 0;

   mNumElements = 0;
   mIndex = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Destructor, deallocate the array

BlockPoolLMIndexStack::~BlockPoolLMIndexStack()
{
   finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This initializes the stack to a fixed size. It initializes member
// variables and and the stack array, given external memory.

void BlockPoolLMIndexStack::initialize(BlockPoolParms* aParms,void* aMemory)
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
      mMemory = malloc(BlockPoolLMIndexStack::getMemorySize(aParms));
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

   char* tArrayMemory = tMemoryPtr.cfetch_add(tMemorySize.mArraySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize variables.

   // Initialize variables.
   mIndex = 0;
   mNumElements = aParms->mNumBlocks;

   // Construct the element array.Call the constructor with placement new.
   mElement = new(tArrayMemory) int[mNumElements];

   // Push the indices of the blocks in the array onto the index stack.
   // For aAllocate==10 this will push 9,7,8,6,5,4,3,2,1,0
   for (int i = mNumElements - 1; i >= 0; i--)
   {
      push(i);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Deallocate memory for the stack.

void BlockPoolLMIndexStack::finalize()
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
// Push a value onto the stack. Return false if the stack is full.

bool BlockPoolLMIndexStack::push(int aValue)
{
   // Enter critical section.
   mSynchLock.lock();

   // Guard for stack full.
   if (mIndex == mNumElements)
   {
      mSynchLock.unlock();
      return false;
   }

   // Copy the source element to the element at the stack index.
   mElement[mIndex] = aValue;
   // Increment the index.
   mIndex++;

   // Leave critical section.
   mSynchLock.unlock();

   // Done
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pop a value off of the stack. Return false if the stack is empty.

bool BlockPoolLMIndexStack::pop(int* aValue)
{
   // Enter critical section.
   mSynchLock.lock();

   // Guard for stack empty.
   if (mIndex == 0)
   {
      mSynchLock.unlock();
      return false;
   }

   // Copy the element below the stack index to the destination element.
   *aValue = mElement[mIndex - 1];
   // Decrement the index.
   mIndex--;

   // Leave critical section.
   mSynchLock.unlock();

   // Return success.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return size.

int BlockPoolLMIndexStack::size()
{
   return mIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace