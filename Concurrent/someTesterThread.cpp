/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someShare.h"
#include "Parms.h"

#define  _SOMETESTERTHREAD_CPP_
#include "someTesterThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TesterThread::TesterThread() 
{
   // Settings Members
   mSleepLower = gParms.mSleepLower;
   mSleepUpper = gParms.mSleepUpper;
   mWriteLower = gParms.mWriteLower;
   mWriteUpper = gParms.mWriteUpper;

   // BaseClass
   BaseClass::setThreadPriorityHigh();

   BaseClass::mThreadSingleProcessor = 1;

   // Thread Members
   mTerminateFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TesterThread::threadInitFunction()
{
   Prn::print(0,"TesterThread::threadInitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TesterThread::threadRunFunction()
{
   try
   {
      gShare.mTester.startTrial();
      while (1)
      {
         // Sleep
         threadSleep(my_irand(mSleepLower, mSleepUpper));
         // Thread loop termination
         if (gParms.mTerminate != 0)
         {
            if (gShare.mTester.mCount > gParms.mTerminate)
            {
               gShare.mTerminateFlag = true;
            }
         }
         if (mTerminateFlag) break;
         if (gShare.mTerminateFlag) break;

         // Write
         gShare.mWriterProcessor[0] = BaseClass::getThreadProcessorNumber();
         gShare.mTester.test(my_irand(mWriteLower, mWriteUpper));
      }
      gShare.mTester.finishTrial();
   }
   catch (...)
   {
      Ris::Threads::halt("TesterThread::exception");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TesterThread::threadExitFunction()
{
   gShare.mTester.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TesterThread::shutdownThread()
{
   // Set terminate
   mTerminateFlag = true;
   // Wait for terminate
   waitForThreadTerminate();
}   

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

