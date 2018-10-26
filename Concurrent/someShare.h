#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risTimeMarker.h"
#include "someWriter.h"
#include "someReader.h"
#include "someTester.h"
#include "ccSRSWValueQueue.h"
#include "ccSRSWObjectQueue.h"
#include "ccLCObjectQueue.h"
#include "ccLCValueQueue.h"
#include "ccLCPointerQueue.h"
#include "someClass1.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class Share
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxNumWriters = 10;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   int mMode;
   int mType;
   int mTest;
   bool mTerminateFlag;

   int            mNumWriters;

   Writer         mWriter[cMaxNumWriters];
   Reader         mReader;
   Tester         mTester;

   int mWriterProcessor[cMaxNumWriters];
   int mReaderProcessor;

   unsigned long long  mWriterCount;
   unsigned long long  mWriterPassCount;
   unsigned long long  mWriterFailCount;
   unsigned long long  mWriterCheckSum;
   double              mWriterMeanTime;

   unsigned long long  mReaderCount;
   unsigned long long  mReaderPassCount;
   unsigned long long  mReaderFailCount;
   unsigned long long  mReaderCheckSum;
   double              mReaderMeanTime;

   double              mWriterMeanTimePop;
   double              mWriterMeanTimePush;

   unsigned long long  mTesterCount;
   double              mTesterMeanTime1;
   double              mTesterMeanTime2;

   CC::SRSWValueQueue<int, 10000>      mSRSWIntQueue;
   CC::SRSWObjectQueue<Class1A, 10000> mSRSWObjectQueue;

   CC::LCObjectQueue             mLCObjectQueue;
   CC::LCValueQueue<int>         mLCIntQueue;
   CC::LCPointerQueue            mLCPointerQueue;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   Share();
   void initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   void update1();
   void update2();
   void update3();
   void update();

   void show12();
   void show3();
   void show();

   void putDummy(void* aPointer);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _SOMESHARE_CPP_
          Share gShare;
#else
   extern Share gShare;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
