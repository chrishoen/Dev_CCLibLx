/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "ccBlockPool.h"
#include "someBlockPoolIndex.h"

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
            case 1:  tQueueSize = gShare.mSRSWIntQueue.size(); break;
            case 2:  tQueueSize = gShare.mSRSWObjectQueue.size(); break;
            case 3:  tQueueSize = gShare.mLCPointerQueue.size(); break;
            case 6:  tQueueSize = gShare.mSRSWIntQueue.size(); break;
            case 7:  tQueueSize = gShare.mLCObjectQueue.size(); break;
            case 8:  tQueueSize = gShare.mLCIntQueue.size(); break;
            case 9:  tQueueSize = CC::getBlockPoolSize(cBlockPoolIndex_MyBlockX); break;
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

