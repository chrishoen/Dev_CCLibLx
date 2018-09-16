/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "LFIntQueue.h"
#include "LFFreeList.h"
#include "Parms.h"

#define  _SOMESHARE_CPP_
#include "someShare.h"
namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

Share::Share()
{
   initialize();
}

void Share::initialize()
{
   mMode = gParms.mMode;
   mType = gParms.mType;
   mTest = gParms.mTest;
   mTerminateFlag = false;

   switch (mMode)
   {
   case 1:
   case 2:
      switch (mType)
      {
      case 1:
         LFIntQueue::initialize(gParms.mNumElements);
         break;
      case 2:
         mLFPointerQueue.initialize(gParms.mNumElements);
         break;
      case 3:
         mLFObjectQueue.initialize(gParms.mNumElements, sizeof(Class1A));
         break;
      case 4:
         mLFValueQueue.initialize(gParms.mNumElements);
         break;
      case 5:
         mLFIntQueue.initialize(gParms.mNumElements);
         break;
      case 6:
         mSRSWIntQueue.initialize(gParms.mNumElements);
         break;
      case 7:
         mLMObjectQueue.initialize(gParms.mNumElements, sizeof(Class1A));
         break;
      case 8:
         mLMIntQueue.initialize(gParms.mNumElements);
         break;
      case 9:
         mLMPointerQueue.initialize(gParms.mNumElements);
         break;
      case 10:
         mLMPointerQueue.initialize(gParms.mNumElements);
         break;
      case 11:
         break;
      case 12:
         break;
      case 21:
      case 22:
         mLFValueQueue.initialize(gParms.mNumElements);
         break;
      }
      break;
   }

   mNumWriters = gParms.mNumWriters;
   if (mNumWriters > cMaxNumWriters) mNumWriters = cMaxNumWriters;

   for (int i = 0; i < mNumWriters; i++)
   {
      mWriter[i].initialize(i);
      mWriterReader[i].initialize(i);
      mWriterProcessor[i]=0;
   }

   mReader.initialize();
   mReaderProcessor=0;

   mTester.initialize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Share::update1()
{
   mWriterCount     = 0;
   mWriterPassCount = 0;
   mWriterFailCount = 0;
   mWriterCheckSum  = 0;
   mWriterMeanTime  = 0.0;
   mWriterMeanTimePop  = 0.0;
   mWriterMeanTimePush  = 0.0;

   for (int i = 0; i < mNumWriters; i++)
   {
      mWriterCount     += mWriter[i].mCount;
      mWriterPassCount += mWriter[i].mPassCount;
      mWriterFailCount += mWriter[i].mFailCount;
      mWriterCheckSum  += mWriter[i].mCheckSum;
      mWriterMeanTime  += mWriter[i].mMeanTimeWrite/mNumWriters;
      mWriterMeanTimePop   += mWriter[i].mMeanTimePop/mNumWriters;
      mWriterMeanTimePush  += mWriter[i].mMeanTimePush/mNumWriters;
   }

   mReaderCount     = mReader.mCount;
   mReaderPassCount = mReader.mPassCount;
   mReaderFailCount = mReader.mFailCount;
   mReaderCheckSum  = mReader.mCheckSum;
   mReaderMeanTime  = mReader.mMeanTimeRead;
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Share::update2()
{
   mWriterCount     = 0;
   mWriterPassCount = 0;
   mWriterFailCount = 0;
   mWriterCheckSum  = 0;
   mWriterMeanTime  = 0.0;

   for (int i = 0; i < mNumWriters; i++)
   {
      mWriterCount     += mWriterReader[i].mWriteCount;
      mWriterPassCount += mWriterReader[i].mWritePassCount;
      mWriterFailCount += mWriterReader[i].mWriteFailCount;
      mWriterCheckSum  += mWriterReader[i].mWriteCheckSum;
      mWriterMeanTime  += mWriterReader[i].mMeanTimeWrite/mNumWriters;
   }

   mReaderCount     = 0;
   mReaderPassCount = 0;
   mReaderFailCount = 0;
   mReaderCheckSum  = 0;
   mReaderMeanTime  = 0.0;

   for (int i = 0; i < mNumWriters; i++)
   {
      mReaderCount     += mWriterReader[i].mReadCount;
      mReaderPassCount += mWriterReader[i].mReadPassCount;
      mReaderFailCount += mWriterReader[i].mReadFailCount;
      mReaderCheckSum  += mWriterReader[i].mReadCheckSum;
      mReaderMeanTime  += mWriterReader[i].mMeanTimeRead/mNumWriters;
   }
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Share::update3()
{
   mTesterCount     = mTester.mCount;
   mTesterMeanTime1  = mTester.mMeanTimeTest1;
   mTesterMeanTime2  = mTester.mMeanTimeTest2;
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Share::update()
{
   switch (gShare.mMode)
   {
   case 1: update1 (); break;
   case 2: update2 (); break;
   case 7: update3 (); break;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Share::show12()
{
   char tString[40];

   Prn::print(0, "");
   Prn::print(0, "");
   Prn::print(0, "");
   Prn::print(0, "TOTAL");
   Prn::print(0, "");
   Prn::print(0, "Writer.mCheckSum   %16llu", mWriterCheckSum);
   Prn::print(0, "Reader.mCheckSum   %16llu", mReaderCheckSum);
   Prn::print(0, "");
   Prn::print(0, "Writer.mCount      %16s", my_stringLLU(tString, mWriterCount));
   Prn::print(0, "Writer.mPassCount  %16s", my_stringLLU(tString, mWriterPassCount));
   Prn::print(0, "Writer.mFailCount  %16s", my_stringLLU(tString, mWriterFailCount));
   Prn::print(0, "");
   Prn::print(0, "Reader.mCount      %16s", my_stringLLU(tString, mReaderCount));
   Prn::print(0, "Reader.mPassCount  %16s", my_stringLLU(tString, mReaderPassCount));
   Prn::print(0, "Reader.mFailCount  %16s", my_stringLLU(tString, mReaderFailCount));
   Prn::print(0, "");

   if (mType==1) LFIntQueue::show();

   Prn::print(0, "");
   Prn::print(0, "Writer.mMeanTime   %16.5f", mWriterMeanTime);
   Prn::print(0, "Reader.mMeanTime   %16.5f", mReaderMeanTime);
   Prn::print(0, "mMeanTime          %16.5f", mReaderMeanTime + mWriterMeanTime);

   double tWriterPass = (double)mWriterPassCount / (double)mWriterCount;
   double tReaderPass = (double)mReaderPassCount / (double)mReaderCount;
   double tWriteRetry = (double)LFIntQueue::writeRetry() / (double)mWriterCount;
   double tReadRetry = (double)LFIntQueue::readRetry() / (double)mReaderCount;
   double tPopRetry = (double)LFIntQueue::popRetry() / (double)mWriterPassCount;
   double tPushRetry = (double)LFIntQueue::pushRetry() / (double)mReaderPassCount;

   Prn::print(0, "");
   Prn::print(0, "WriterPass         %16.5f", tWriterPass);
   Prn::print(0, "ReaderPass         %16.5f", tReaderPass);
   Prn::print(0, "writeRetry         %16.5f", tWriteRetry);
   Prn::print(0, "readRetry          %16.5f", tReadRetry);
   Prn::print(0, "popRetry           %16.5f", tPopRetry);
   Prn::print(0, "pushRetry          %16.5f", tPushRetry);

   Prn::print(0, "");
   for (int i = 0; i < mNumWriters; i++)
   {
      mWriter[i].mMarkerWrite.mStatistics.showEUMM(0, "WStat");
   }
   mReader.mMarkerRead.mStatistics.showEUMM(0, "RStat");

}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Share::show3()
{
   char tString[40];

   Prn::print(0, "");
   Prn::print(0, "");
   Prn::print(0, "");
   Prn::print(0, "TOTAL");
   Prn::print(0, "");
   Prn::print(0, "Tester.mCount      %16s", my_stringLLU(tString, mTesterCount));
   Prn::print(0, "mMeanTimeTest1     %16.5f", mTesterMeanTime1);
   Prn::print(0, "mMeanTimeTest2     %16.5f", mTesterMeanTime2);
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Share::show()
{
   switch (gShare.mMode)
   {
   case 1: show12 (); break;
   case 2: show12 (); break;
   case 3: show3  (); break;
   }
}

static int gDummyCount=0;
void Share::putDummy(void* aPointer)
{
   gDummyCount++;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
