#include "stdafx.h"

#include "risTimeMarker.h"
#include "Parms.h"
#include "LFFreeList.h"
#include "LFIntQueue.h"
#include "LFBackoff.h"
#include "Timing.h"

using namespace std;

namespace Timing
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Regionals.

   Ris::TrialTimeMarker mMarker;
   Ris::TrialTimeMarker mMarkerWrite;
   Ris::TrialTimeMarker mMarkerRead;
   Ris::TrialTimeMarker mMarkerPop;
   Ris::TrialTimeMarker mMarkerPush;
   int mWriteCount=0;
   int mReadCount=0;
   int mPopCount=0;
   int mPushCount=0;

   std::atomic<int> mAX;
   int mNC=0;
   int mNE=0;

   void test11();
   void test12();
   void test13();
   void test14();
   void test15();
   void test16();

   void test21();
   void test22();

   void test81();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Run1

   void run1(int aTest)
   {
      int tIterations = 1000000;

      mWriteCount=0;
      mReadCount=0;
      mAX=0;

      mMarker.startTrial(gParms.mXLimit);

      for (int i = 0; i < tIterations; i++)
      {
         mMarker.doStart();
         switch (aTest)
         {
         case 1:  test11(); break;
         case 2:  test12(); break;
         case 3:  test13(); break;
         case 4:  test14(); break;
         case 5:  test15(); break;
         case 6:  test16(); break;
         }
         mMarker.doStop();
      }

      mMarker.finishTrial();

      Prn::print(0, "TEST1 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         mMarker.mStatistics.mPutCount,
         mMarker.mStatistics.mMean,
         mMarker.mStatistics.mStdDev,
         mMarker.mStatistics.mMinX,
         mMarker.mStatistics.mMaxX);

      Prn::print(0, "Done");
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Run2

   void run2(int aTest)
   {
      int tIterations = 1000000;

      mWriteCount=0;
      mReadCount=0;

      mMarkerWrite.startTrial(gParms.mXLimit);
      mMarkerRead.startTrial(gParms.mXLimit);

      for (int i = 0; i < tIterations; i++)
      {
         switch (aTest)
         {
         case 1:  test21(); break;
         case 2:  test22(); break;
         }
      }

      mMarkerWrite.finishTrial();
      mMarkerRead.finishTrial();

      Prn::print(0, "WRITE2 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         mMarkerWrite.mStatistics.mPutCount,
         mMarkerWrite.mStatistics.mMean,
         mMarkerWrite.mStatistics.mStdDev,
         mMarkerWrite.mStatistics.mMinX,
         mMarkerWrite.mStatistics.mMaxX);

      Prn::print(0, "READ2  %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         mMarkerRead.mStatistics.mPutCount,
         mMarkerRead.mStatistics.mMean,
         mMarkerRead.mStatistics.mStdDev,
         mMarkerRead.mStatistics.mMinX,
         mMarkerRead.mStatistics.mMaxX);

      Prn::print(0, "Done");
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Run8

   void run8(int aTest)
   {
      int tIterations = 1000000;

      mPopCount=0;
      mPushCount=0;

      mMarkerPop.startTrial(gParms.mXLimit);
      mMarkerPush.startTrial(gParms.mXLimit);

      for (int i = 0; i < tIterations; i++)
      {
         switch (aTest)
         {
         case 1:  test81(); break;
         }
      }

      mMarkerPop.finishTrial();
      mMarkerPush.finishTrial();

      Prn::print(0, "POP8   %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         mMarkerPop.mStatistics.mPutCount,
         mMarkerPop.mStatistics.mMean,
         mMarkerPop.mStatistics.mStdDev,
         mMarkerPop.mStatistics.mMinX,
         mMarkerPop.mStatistics.mMaxX);

      Prn::print(0, "PUSH8  %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         mMarkerPush.mStatistics.mPutCount,
         mMarkerPush.mStatistics.mMean,
         mMarkerPush.mStatistics.mStdDev,
         mMarkerPush.mStatistics.mMinX,
         mMarkerPush.mStatistics.mMaxX);

      Prn::print(0, "Done");
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Test1

   void test11()
   {
   }

   void test12()
   {
   }

   void test13()
   {
      mWriteCount++;
   }

   void test14()
   {
      mAX++;

      mAX = 100;
      mNC = 200;
      mNE = 200;

      mAX.compare_exchange_weak(mNC,mNE);
   }

   void test15()
   {
      LFIntQueue::tryWrite(++mWriteCount);
      LFIntQueue::tryRead(&mReadCount);
   }

   void test16()
   {
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Test2

   void test21()
   {
      mMarkerWrite.doStart();
      LFIntQueue::tryWrite(++mWriteCount);
      mMarkerWrite.doStop();

      mMarkerRead.doStart();
      LFIntQueue::tryRead(&mReadCount);
      mMarkerRead.doStop();
   }

   void test22()
   {
      mMarkerWrite.doStart();
      LFIntQueue::tryWrite(++mWriteCount);
      mMarkerWrite.doStop();

      mMarkerRead.doStart();
      LFIntQueue::tryRead(&mReadCount);
      mMarkerRead.doStop();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Test8

   void test81()
   {
      int tNode;
      bool tPass;

      mMarkerPop.doStart();
      tPass = LFFreeList::listPop(&tNode);
      mMarkerPop.doStop();

      if (tPass)
      {
         mMarkerPush.doStart();
         LFFreeList::listPush(tNode);
         mMarkerPush.doStop();
      }
   }

//***************************************************************************
//***************************************************************************
//***************************************************************************
}//namespace