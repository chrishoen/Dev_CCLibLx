/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someShare.h"
#include "Parms.h"

#define  _SOMEWRITERTHREAD_CPP_
#include "someWriterThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

WriterThread::WriterThread(int aIdent) 
{
   // Settings Members
   mSleepLower = gParms.mSleepLower;
   mSleepUpper = gParms.mSleepUpper;
   mWriteLower = gParms.mWriteLower;
   mWriteUpper = gParms.mWriteUpper;

   // BaseClass
   BaseClass::setThreadPriorityHigh();

   switch (aIdent)
   {
   case 0:
      BaseClass::mThreadSingleProcessor = 2;
      break;
   case 1:
      BaseClass::mThreadSingleProcessor = 3;
      break;
   case 2:
      BaseClass::mThreadSingleProcessor = 4;
      break;
   case 3:
      BaseClass::mThreadSingleProcessor = 5;
      break;
   }

   // Thread Members
   mTerminateFlag = false;
   mIdent = aIdent;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterThread::threadInitFunction()
{
   Prn::print(0,"WriterThread::threadInitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterThread::threadRunFunction()
{
   try
   {
      gShare.mWriter[mIdent].startTrial();
      int tLoopCount = 0;
      while (1)
      {
         // Start trial statistics after cache settles.
         if (tLoopCount == 1000)
         {
            gShare.mWriter[mIdent].finishTrial();
            gShare.mWriter[mIdent].startTrial();
         }
         // Sleep
         threadSleep(my_irand(mSleepLower, mSleepUpper));
         // Thread loop termination
         if (gParms.mTerminate != 0)
         {
            if (gShare.mWriter[mIdent].mCount > gParms.mTerminate)
            {
               gShare.mTerminateFlag = true;
            }
         }
         if (mTerminateFlag) break;
         if (gShare.mTerminateFlag) break;

         // Write
         gShare.mWriterProcessor[mIdent] = BaseClass::getThreadProcessorNumber();
         gShare.mWriter[mIdent].write(my_irand(mWriteLower, mWriteUpper));
         tLoopCount++;
      }
      gShare.mWriter[mIdent].finishTrial();
   }
   catch (...)
   {
      Ris::Threads::halt("WriterThread::exception");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterThread::threadExitFunction()
{
   gShare.mWriter[mIdent].show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterThread::shutdownThread()
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

