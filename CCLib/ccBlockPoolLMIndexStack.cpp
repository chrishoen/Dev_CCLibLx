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
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor, initialize members for an empty stack of size zero. 

int BlockPoolLMIndexStackState::getMemorySize()
{
   return cc_round_upto16(sizeof(BlockPoolLMIndexStackState));
}

BlockPoolLMIndexStackState::BlockPoolLMIndexStackState()
{
   // All null.
   mNumElements = 0;
   mIndex       = 0;
}

void BlockPoolLMIndexStackState::initialize(BlockPoolParms* aParms)
{
   // Do not initialize, if already initialized.
   if (!aParms->mConstructorFlag) return;

   // Initialize variables.
   mIndex = 0;
   mNumElements = aParms->mNumBlocks;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This local class calculates and stores the memory sizes needed by the class.

class BlockPoolLMIndexStack::MemorySize
{
public:
   // Members.
   int mStateSize;
   int mArraySize;
   int mMemorySize;

   // Calculate and store memory sizes.
   MemorySize(BlockPoolParms* aParms)
   {
      mStateSize = BlockPoolLMIndexStackState::getMemorySize();
      mArraySize = cc_round_upto16(cNewArrayExtraMemory + aParms->mNumBlocks*sizeof(int));
      mMemorySize = mStateSize + mArraySize;
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
   // All null.
   mX = 0;
   mElement = 0;
   mOwnMemoryFlag = false;
   mMemory = 0;
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

   char* tStateMemory = tMemoryPtr.cfetch_add(tMemorySize.mStateSize);
   char* tArrayMemory = tMemoryPtr.cfetch_add(tMemorySize.mArraySize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize variables.

   // Construct the state.
   if (aParms->mConstructorFlag)
   {
      // Call the constructor.
      mX = new(tStateMemory)BlockPoolLMIndexStackState;
   }
   else
   {
      // The constructor has already been called.
      mX = (BlockPoolLMIndexStackState*)tStateMemory;
   }
   // Initialize the state.
   mX->initialize(aParms);

   // Construct the element array.
   if (aParms->mConstructorFlag)
   {
      // Call the constructor.
      mElement = new(tArrayMemory) int[mX->mNumElements];
   }
   else
   {
      // The constructor has already been called.
      mElement = (int*)tArrayMemory;
   }

   // Initialize the element array, if it has not already been initialized.
   if (aParms->mConstructorFlag)
   {
      // Push the indices of the blocks in the array onto the index stack.
      // For aAllocate==10 this will push 9,7,8,6,5,4,3,2,1,0
      for (int i = mX->mNumElements - 1; i >= 0; i--)
      {
         push(i);
      }
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
   if (mX->mIndex == mX->mNumElements)
   {
      mSynchLock.unlock();
      return false;
   }

   // Copy the source element to the element at the stack index.
   mElement[mX->mIndex] = aValue;
   // Increment the index.
   mX->mIndex++;

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
   if (mX->mIndex == 0)
   {
      mSynchLock.unlock();
      return false;
   }

   // Copy the element below the stack index to the destination element.
   *aValue = mElement[mX->mIndex - 1];
   // Decrement the index.
   mX->mIndex--;

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
   return mX->mIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace