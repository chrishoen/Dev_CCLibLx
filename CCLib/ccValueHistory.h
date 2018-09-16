#pragma once


/*==============================================================================

Template for a class that implements a container that stores a history of 
values and times.

==============================================================================*/

#include <stdio.h>
#include <stdlib.h>

#include "cc_functions.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a class template for a history of values. It contains a circular
// buffer of values and a circualr buffer of times. Values and times are put
// to the container and can be gotten according to an offset index. 

template <class Time,class Element>
class ValueHistory
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   int mNumElements;
   int mIndex;
   int mSize;

   Time*    mTime;
   Element* mElement;
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor, initialize members for an empty History of size zero 
   ValueHistory()
   {
      // All null
      mNumElements = 0;
      mIndex = 0;
      mSize = 0;
      mTime = 0;
      mElement  = 0;
   }

   // Destructor, deallocate the History array
  ~ValueHistory()
   {
     finalize();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initializes the history to a fixed size. It initializes member
   // variables and allocates heap storage for the history array. The history has
   // a specified maximum number of elements and it allocates memory for the
   // maximum number of elements.

  void initialize(int aNumOfElements)
   {
      // Deallocate memory, if any exists.
      finalize();

      // Allocate memory for the array to have an extra element
      mNumElements = aNumOfElements;

      // Allocate memory for the array
      mTime    = new Time[mNumElements];
      mElement = new Element[mNumElements];

      // Initialize variables
      mIndex = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Reinitializes the history. It is equivalent to an initialize without
   // the memory allocation.

  void reinitialize()
   {
      // Initialize variables
      mIndex = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // If memory has been allocated then deallocate it.

   void finalize()
   {
      if (mElement)
      {
         free(mTime);
         free(mElement);
      }
      mTime = 0;
      mElement = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Return the size.

   int  size()    { return mSize; }
   bool isGet()   { return mSize != 0; }
   bool isEmpty() { return mSize == 0; }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Put an element to the history and advance the index and the size.

   void put (Time aTime,Element aElement)
   {
      // Store time and element at the current index.
      mTime    [mIndex] = aTime;
      mElement [mIndex] = aElement;

      // Advance the index. This wraps around in a circular manner.
      mIndex = cc_index_add(mIndex, 1, mNumElements);

      // Advance the size.  0 <= mSize <= mNumElements.
      if (mSize < mNumElements) mSize++;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Get a time from the history at an index that is offset from the
   // current index.

   Time getTime(int aOffset)
   {
      // Calculate index to get from. It is the current index minus the offset
      // plus one, wrapping around in a cirular manner.
      int tGetIndex = cc_index_sub(mIndex, aOffset + 1, mNumElements);

      // Return Time at get index.
      return mTime[tGetIndex];
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Get an element from the history at an index that is offset from the
   // current index.

   Element getElement(int aOffset)
   {
      // Calculate index to get from. It is the current index minus the offset
      // plus one, wrapping around in a cirular manner.
      int tGetIndex = cc_index_sub(mIndex, aOffset + 1, mNumElements);

      // Return element at get index.
      return mElement[tGetIndex];
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
