#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <atomic>
#include "ccDefs.h"

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Lock Free Index used by CAS based algorithms.

#pragma pack(push,1)
typedef struct alignas(4) LFIndex
{
   short int mIndex;
   short int mCount;

   LFIndex() noexcept
   {
      mIndex = 0;
      mCount = 0;
   }

   LFIndex(short int aIndex, short int aCount) noexcept
   {
      mIndex = aIndex;
      mCount = aCount;
   }
} LFIndex;
#pragma pack(pop)

// Operator
inline bool operator==(const LFIndex& lhs, const LFIndex& rhs)
{
   return lhs.mIndex == rhs.mIndex && lhs.mCount == rhs.mCount;
}

// Operator
inline bool operator!=(const LFIndex& lhs, const LFIndex& rhs)
{
   return lhs.mIndex != rhs.mIndex && lhs.mCount != rhs.mCount;
}

// Atomic
typedef std::atomic<LFIndex> AtomicLFIndex;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class for lock free container blocks. This is inherited by classes that
// are contained in intrusive lock free containers.

class BaseLFBlock
{
public:
   int mLFNodeIndex;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
