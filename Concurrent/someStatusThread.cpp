/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "ccBlockPool.h"
#include "someBlockPoolIndex.h"

#include "LFFreeList.h"
#include "LFIntQueue.h"
#include "someShare.h"
#include "Parms.h"

#define  _SOMESTATUSTHREAD_CPP_
#include "someStatusThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

StatusThread::StatusThread() 
{
   // Thread Members
   mTerminateFlag = false;

   //Support
   mProcString[0]=0;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void StatusThread::threadInitFunction()
{
   Prn::print(0,"StatusThread::threadInitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void StatusThread::threadRunFunction()
{
   char tString1[40];
   char tString2[40];
   while(1)
   {
      threadSleep(1000);
      if (mTerminateFlag) break;
      if (gShare.mTerminateFlag) break;

      updateProcString();
      switch (gShare.mMode)
      {
         case 1:
         {
            int tQueueSize;
            switch (gShare.mType)
            {
            case 1:  tQueueSize = LFIntQueue::size(); break;
            case 2:  tQueueSize = gShare.mLFPointerQueue.size(); break;
            case 3:  tQueueSize = gShare.mLFObjectQueue.size(); break;
            case 4:  tQueueSize = CC::getBlockPoolSize(cBlockPoolIndex_MyBlockX); break;
            case 5:  tQueueSize = gShare.mLFIntQueue.size(); break;
            case 6:  tQueueSize = gShare.mSRSWIntQueue.size(); break;
            case 7:  tQueueSize = gShare.mLMObjectQueue.size(); break;
            case 8:  tQueueSize = gShare.mLMIntQueue.size(); break;
            case 9:  tQueueSize = CC::getBlockPoolSize(cBlockPoolIndex_MyBlockX); break;
            case 10: tQueueSize = CC::getBlockPoolSize(cBlockPoolIndex_MyBlockX); break;
            }

            Prn::print(Prn::ThreadRun1, "%s$   %s   %s %8d",
               mProcString,
               my_stringLLU(tString1, gShare.mWriter[0].mCount),
               my_stringLLU(tString2, gShare.mReader.mCount),
               tQueueSize);
         }
         break;
         case 2:
         {
            int tQueueSize;
            switch (gShare.mType)
            {
            case 1:  tQueueSize = LFIntQueue::size();
            case 2:  tQueueSize = gShare.mLFPointerQueue.size();
            case 3:  tQueueSize = gShare.mLFObjectQueue.size();
            case 4:  tQueueSize = CC::getBlockPoolSize(cBlockPoolIndex_MyBlockX);
            case 5:  tQueueSize = gShare.mLFIntQueue.size();
            case 6:  tQueueSize = gShare.mSRSWIntQueue.size();
            case 7:  tQueueSize = gShare.mLMObjectQueue.size();
            case 8:  tQueueSize = gShare.mLMIntQueue.size(); break;
            case 11: tQueueSize = 9911;
            case 12: tQueueSize = CC::getBlockPoolSize(cBlockPoolIndex_MyBlockX);
            case 21: tQueueSize = 9921;
            case 22: tQueueSize = CC::getBlockPoolSize(cBlockPoolIndex_MyBlockX);
            }

            Prn::print(Prn::ThreadRun1, "%s$   %s   %s %8d",
               mProcString,
               my_stringLLU(tString1, gShare.mWriterReader[0].mWriteCount),
               my_stringLLU(tString2, gShare.mWriterReader[0].mReadCount),
               tQueueSize);
         }
         break;
         case 3:
         {
            Prn::print(Prn::ThreadRun1, "%s$%d   %s",
               mProcString,
               gShare.mTest,
               my_stringLLU(tString1, gShare.mTester.mCount));
         }
         break;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void StatusThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void StatusThread::shutdownThread()
{
   // Set terminate
   mTerminateFlag = true;
   // Wait for terminate
   waitForThreadTerminate();
}   

//******************************************************************************
//******************************************************************************
//******************************************************************************

void StatusThread::updateProcString()
{
   for (int i = 0; i < gShare.mNumWriters; i++)
   {
      mProcString[i] = '0' + gShare.mWriterProcessor[i];
   }
   mProcString[gShare.mNumWriters] = '0' + gShare.mReaderProcessor;
   mProcString[gShare.mNumWriters+1] = 0;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

