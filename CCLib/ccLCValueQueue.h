#pragma once

/*==============================================================================

Single Reader Multiple Writer Value Queue class template. 

It is multiple writer single reader thread safe.
It lockes with critcal sections for mutual exclusion, blocking.
It is shared memory safe.

This implements a queue of fixed size objects, where the queue provides the
memory allocation for the objects (memory for the objects is contained
within the queue). The queue is thread safe. It uses mutexes to guard
against concurrency contentions.

It is thread safe for separate multiple writer and single reader threads.

It implements the Michael and Scott algorithm for blocking queues. It
uses critcal sections for mutex protection. It maintains storage for the
objects by implementing a free list that also uses critical section mutex
protection.



==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <new>
#include "ccDefs.h"
#include "cc_functions.h"
#include "ccMemoryPtr.h"
#include "ccCriticalSection.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CC
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// State variables for the stack. These are located in a separate class
// so that they can be located in external memory.

class LCValueQueueState
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Class.

   // This returns the number of bytes that an instance of this class
   // will need to be allocated for it.
   static int getMemorySize()
   {
      return cc_round_upto16(sizeof(LCValueQueueState));
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Number of elements allocated.
   int mNumElements;
   int mPutIndex;
   int mGetIndex;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   LCValueQueueState()
   {
      // All null
      mNumElements = 0;
      mPutIndex = 0;
      mGetIndex = 0;
   }

   // Initialize.
   void initialize(int aNumElements,bool aConstructorFlag)
   {
      // Do not initialize, if already initialized.
      if (!aConstructorFlag) return;

      // Allocate for one extra dummy node.
      mNumElements = aNumElements + 1;
      mPutIndex = 0;
      mGetIndex = 0;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

template <class Element>
class LCValueQueue
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Class.

   // This local class calculates and stores the memory sizes needed by the class.
   class MemorySize
   {
   public:
      // Members.
      int mStateSize;
      int mElementArraySize;
      int mMemorySize;

      // Calculate and store memory sizes.
      MemorySize(int aNumElements)
      {
         mStateSize         = LCValueQueueState::getMemorySize();
         mElementArraySize  = cc_round_upto16(cNewArrayExtraMemory + (aNumElements + 1)*sizeof(Element));
         mMemorySize = mStateSize + mElementArraySize;
      }
   };

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Class.

   // This returns the number of bytes that an instance of this class
   // will need to be allocated for it.
   static int getMemorySize(int aNumElements)
   {
      MemorySize tMemorySize(aNumElements);
      return tMemorySize.mMemorySize;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If this flag is false then the memory for this object was created
   // externally. If it is true then the memory was allocated at 
   // initialization and must be freed at finalization.
   bool mOwnMemoryFlag;

   // Pointer to memory for which the queue resides. This is either created
   // externally and passed as an initialization parameter or it is created
   // on the system heap at initialization.
   void* mMemory;

   // State variables for the queue. These are located in a separate class
   // so that they can be located in externale memory.
   LCValueQueueState* mX;

   // Array of values, storage for the values.
   // Size is NumElements + 1.
   // Index range is 0..NumElements.
   Element* mElement;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Critical section.
   void* mCriticalSection;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   LCValueQueue()
   {
      // All null.
      mX = 0;
      mOwnMemoryFlag = false;
      mMemory = 0;

      // All null
      mElement = 0;

      mCriticalSection = createCriticalSection();
   }

   ~LCValueQueue()
   {
      finalize();
      destroyCriticalSection(mCriticalSection);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   void initialize(int aNumElements)
   {
      initialize(aNumElements,true,0);
   }

   void initialize(int aNumElements,bool aConstructorFlag, void* aMemory)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Initialize memory.

      // Deallocate memory, if any exists.
      finalize();

      // If the instance of this class is not to reside in external memory
      // then allocate memory for it on the system heap.
      if (aMemory == 0)
      {
         mMemory = malloc(LCValueQueue<Element>::getMemorySize(aNumElements));
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
      MemorySize tMemorySize(aNumElements);

      // Calculate memory addresses.
      MemoryPtr tMemoryPtr(mMemory);

      char* tStateMemory        = tMemoryPtr.cfetch_add(tMemorySize.mStateSize);
      char* tElementArrayMemory = tMemoryPtr.cfetch_add(tMemorySize.mElementArraySize);

      // Construct the state.
      if (aConstructorFlag)
      {
         // Call the constructor.
         mX = new(tStateMemory)LCValueQueueState;
      }
      else
      {
         // The constructor has already been called.
         mX = (LCValueQueueState*)tStateMemory;
      }
      // Initialize the state.
      mX->initialize(aNumElements,aConstructorFlag);

      // Construct the arrays.
      if (aConstructorFlag)
      {
         // Call the constructor.
         mElement = new(tElementArrayMemory)Element[mX->mNumElements];
      }
      else
      {
         // The constructor has already been called.
         mElement = (Element*)tElementArrayMemory;
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Initialize variables.
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Finalize.

   void finalize()
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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This is the current size of the queue. It is the difference between the 
   // mPutIndex and the mGetIndex.

   int size()
   {
      int tSize = mX->mPutIndex - mX->mGetIndex;
      if (tSize < 0) tSize = mX->mNumElements + tSize;
      return tSize;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This attempts to write a value to the queue. If the queue is not full
   // then it succeeds.
   // 
   // This tests if put operations are allowed. Puts are allowed if the 
   // current size is less than or equal to Allocate - 2. If the size is equal
   // to Allocate - 2 then the next put operation would put the size to
   // mNumElements - 1, which is the max number of elements. This is the same
   // as "is not full".
   // 
   // This puts an element to the queue and advances the put index. It does a 
   // copy from a source element into the queue array element at the put index.
   // It uses a temp index variable so that writing to the index is atomic.

   bool tryWrite (Element aElement)
   {
      // Lock.
      enterCriticalSection(mCriticalSection);

      // Test if the queue is full.
      int tSize = mX->mPutIndex - mX->mGetIndex;
      if (tSize < 0) tSize = mX->mNumElements + tSize;
      if (tSize > mX->mNumElements - 2)
      {
         // Unlock.
         leaveCriticalSection(mCriticalSection);
         return false;
      }

      // Local put index.
      int tPutIndex = mX->mPutIndex;
      // Copy the source element into the element at the queue put index
      mElement[tPutIndex] = aElement;
      // Advance the put index.
      if(++tPutIndex == mX->mNumElements) tPutIndex = 0;
      mX->mPutIndex = tPutIndex;

      // Unlock.
      leaveCriticalSection(mCriticalSection);
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This attempts to read a value to the queue. If the queue is not empty
   // then it succeeds.
   //
   // This gets an element from the queue and advances the get index. It does a 
   // copy from the queue array element at the get index into a destination
   // element. It uses a temp index variable so that writing to the index is
   // atomic. Note that the destination element must be of element size.
  
   bool tryRead(Element* aValue)
   {
      // Test if the queue is empty.
      int tSize = mX->mPutIndex - mX->mGetIndex;
      if (tSize < 0) tSize = mX->mNumElements + tSize;
      if (tSize == 0) return false;

      // Local index
      int tGetIndex = mX->mGetIndex;
      // Copy the queue array element at the get index
      *aValue = mElement[tGetIndex];
      // Advance the get index
      if(++tGetIndex == mX->mNumElements) tGetIndex = 0;
      mX->mGetIndex = tGetIndex;

      // Done.
      return true;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

