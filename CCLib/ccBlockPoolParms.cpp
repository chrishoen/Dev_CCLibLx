/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "ccBlockPoolParms.h"
#include "cc_functions.h"

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This returns the number of bytes that an instance of this class
// will need to be allocated for it.

int BlockPoolParms::getMemorySize()
{
   return cc_round_upto16(sizeof(BlockPoolParms));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Block pool parameters. These are passed to the create block pool call.

BlockPoolParms::BlockPoolParms()
{
   reset();
}

void BlockPoolParms::reset()
{
   mPoolIndex        = 0;
   mBlockPoolType    = cBlockPoolType_LCFreeList;
   mNumBlocks        = 0;
   mBlockSize        = 0;
   mMemory           = 0;
   mNoThrowFlag      = false;

   mBlockBoxSize     = 0;
   mBlockHeaderSize  = 0;
   mValidFlag        = false;
   mBlockBoxArrayPtr.setNull();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Make parameters consistent. Check ranges.

void BlockPoolParms::makeConsistent()
{
   if (mNumBlocks > cBlockPool_MaxNumBlocks)
   {
      mNumBlocks = cBlockPool_MaxNumBlocks;
   }

   mBlockSize    = cc_round_upto16(mBlockSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

