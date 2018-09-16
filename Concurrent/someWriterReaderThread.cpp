/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someShare.h"
#include "Parms.h"

#define  _SOMEWRITERTHREAD_CPP_
#include "someWriterReaderThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

WriterReaderThread::WriterReaderThread(int aIdent) 
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

void WriterReaderThread::threadInitFunction()
{
   Prn::print(0,"WriterReaderThread::threadInitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReaderThread::threadRunFunction()
{
   try
   {
      gShare.mWriterReader[mIdent].startTrial();
      while (1)
      {
         // Sleep
         threadSleep(my_irand(mSleepLower, mSleepUpper));
         // Thread loop termination
         if (gParms.mTerminate != 0)
         {
            if (gShare.mWriterReader[mIdent].mWriteCount > gParms.mTerminate)
            {
               gShare.mTerminateFlag = true;
            }
         }
         if (mTerminateFlag) break;
         if (gShare.mTerminateFlag) break;

         // Write
         gShare.mWriterProcessor[mIdent] = BaseClass::getThreadProcessorNumber();
         gShare.mWriterReader[mIdent].writeread(my_irand(mWriteLower, mWriteUpper));
      }
      gShare.mWriterReader[mIdent].finishTrial();
   }
   catch (...)
   {
      Ris::Threads::halt("WriterReaderThread::exception");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReaderThread::threadExitFunction()
{
   gShare.mWriterReader[mIdent].flush();
   gShare.mWriterReader[mIdent].show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReaderThread::shutdownThread()
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

