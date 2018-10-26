#pragma once

/*==============================================================================

Single Reader Writer Object Queue Class Template. 

It is single writer single reader thread safe.
It is uses no thread synchronization.
It is shared memory safe.

This implements a value queue. 

It is thread safe for separate single writer and single reader threads.

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

template <class Element,int Size>
class SRSWObjectQueue
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Number of elements allocated is size + 1. There is an extra element
   // allocated.
   static const int cNumElements = Size + 1;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Element access indices.
   int mPutIndex;
   int mGetIndex;

   // Array of elements.
   Element mElement[cNumElements];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SRSWObjectQueue()
   {
      reset();
   }

   void reset()
   {
      // Initialize variables.
      mPutIndex = 0;
      mGetIndex = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This is the current size of the queue. It is the difference between the 
   // mPutIndex and the mGetIndex.

   int size()
   {
      int tSize = mPutIndex - mGetIndex;
      if (tSize < 0) tSize = cNumElements + tSize;
      return tSize;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This attempts to write a value to the queue. If the queue is not full
   // then it succeeds.
   // 
   // This tests if put operations are allowed. Puts are allowed if the 
   // current size is less than or equal to NumElements - 2. If the size is equal
   // to NumElements - 2 then the next put operation would put the size to
   // cNumElements - 1, which is the max number of elements. This is the same
   // as "is not full".
   // 
   // This puts an element to the queue and advances the put index. It does a 
   // copy from a source element into the queue array element at the put index.
   // It uses a temp index variable so that writing to the index is atomic.

   Element* startWrite()
   {
      // If the queue is full then return zero.
      int tSize = mPutIndex - mGetIndex;
      if (tSize < 0) tSize = cNumElements + tSize;
      if (tSize > cNumElements - 2) return 0;

      // Return a pointer to the element at the put index.
      return &mElement[mPutIndex];
   }

   void finishWrite()
   {
      int tPutIndex = mPutIndex;
      if (++tPutIndex == cNumElements) tPutIndex = 0;
      mPutIndex = tPutIndex;
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
  
   Element* startRead()
   {
      // If the queue is empty then return zero.
      int tSize = mPutIndex - mGetIndex;
      if (tSize < 0) tSize = cNumElements + tSize;
      if (tSize == 0) return 0;

      // Return a pointer to the element at the get index.
      return &mElement[mGetIndex];
   }

   void finishRead()
   {
      // Local index.
      int tGetIndex = mGetIndex;
      // Advance the get index.
      if (++tGetIndex == cNumElements) tGetIndex = 0;
      mGetIndex = tGetIndex;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

