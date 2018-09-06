#pragma once

#include <atomic>

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

struct LFIndex
{  
   short int mIndex;  
   short int mCount;  

   LFIndex() noexcept
   {
      mIndex = 0;
      mCount = 0;
   }

   LFIndex(short int aIndex,short int aCount) noexcept
   {
      mIndex = aIndex;
      mCount = aCount;
   }
} __attribute__((aligned(4)));

inline bool operator==(const LFIndex& lhs, const LFIndex& rhs)
{
   return lhs.mIndex==rhs.mIndex && lhs.mCount==rhs.mCount;
}

inline bool operator!=(const LFIndex& lhs, const LFIndex& rhs)
{
   return lhs.mIndex!=rhs.mIndex && lhs.mCount!=rhs.mCount;
}

typedef std::atomic<LFIndex> AtomicLFIndex;

struct AtomicLFIndexBlock 
{  
   AtomicLFIndex mX;
   unsigned long long int mPadding[64/8 - 1];
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
