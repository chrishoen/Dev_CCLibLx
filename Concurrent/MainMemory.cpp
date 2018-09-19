#include "stdafx.h"

#include "Parms.h"
#include "ccBlockPool.h"
#include "someBlockPoolIndex.h"
#include "someMyBlockX.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program memory.

void main_memory_initialize()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize block pools.

   // Initialize block pool central facility.
   CC::initializeBlockPoolCentral();

   // Block pool parameters.
   CC::BlockPoolParms tBlockPoolParms;

   // Create block pool.
   tBlockPoolParms.reset();
   tBlockPoolParms.mPoolIndex     = Some::cBlockPoolIndex_MyBlockX;
   tBlockPoolParms.mBlockPoolType = CC::cBlockPoolType_LCFreeList;
   tBlockPoolParms.mNumBlocks     = gParms.mNumElements;
   tBlockPoolParms.mBlockSize     = sizeof(Some::MyBlockX);
   tBlockPoolParms.mNoThrowFlag   = true;
   CC::createBlockPool(&tBlockPoolParms);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize program memory.

void main_memory_finalize()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Finalize block pools.

   // Finalize block pool central facility.
   // This destroys all created block pools.
   CC::finalizeBlockPoolCentral();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
