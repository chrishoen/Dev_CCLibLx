#include "stdafx.h"

#include "ccBlockPool.h"
#include "someBlockPoolIndex.h"
#include "someMyBlockA.h"
#include "someMyBlockB.h"
#include "someMyBlockC.h"
#include "someMyBlockR.h"

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
   tBlockPoolParms.mPoolIndex     = Some::cBlockPoolIndex_MyBlockA;
   tBlockPoolParms.mBlockPoolType = CC::cBlockPoolType_LMFreeList;
   tBlockPoolParms.mNumBlocks     = 1000;
   tBlockPoolParms.mBlockSize     = sizeof(Some::MyBlockA);
   CC::createBlockPool(&tBlockPoolParms);

   // Create block pool.
   tBlockPoolParms.reset();
   tBlockPoolParms.mPoolIndex = Some::cBlockPoolIndex_MyBlockB;
   tBlockPoolParms.mBlockPoolType = CC::cBlockPoolType_LMFreeList;
   tBlockPoolParms.mNumBlocks = 1000;
   tBlockPoolParms.mBlockSize = sizeof(Some::MyBlockB);
   CC::createBlockPool(&tBlockPoolParms);

   // Create block pool.
   tBlockPoolParms.reset();
   tBlockPoolParms.mPoolIndex = Some::cBlockPoolIndex_MyBlockC;
   tBlockPoolParms.mBlockPoolType = CC::cBlockPoolType_LMFreeList;
   tBlockPoolParms.mNumBlocks = 1000;
   tBlockPoolParms.mBlockSize = sizeof(Some::MyBlockC);
   CC::createBlockPool(&tBlockPoolParms);

   // Create block pool.
   tBlockPoolParms.reset();
   tBlockPoolParms.mPoolIndex     = Some::cBlockPoolIndex_MyBlockR;
   tBlockPoolParms.mBlockPoolType = CC::cBlockPoolType_LMFreeList;
   tBlockPoolParms.mNumBlocks     = 1000;
   tBlockPoolParms.mBlockSize     = sizeof(Some::MyBlockR);
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

