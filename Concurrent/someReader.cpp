/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "Parms.h"
#include "someShare.h"
#include "LFBackoff.h"
#include "someMyBlockX.h"
#include "someReader.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

Reader::Reader()
{
   initialize();
}

void Reader::initialize()
{
   mCount     = 0;
   mPassCount = 0;
   mFailCount = 0;
   mCheckSum   = 0;
   mMeanTimeRead  = 0.0;
}

void Reader::finalize()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::show()
{
   Prn::print(0,"Reader.mCount     %llu",mCount);
   return;
   Prn::print(0,"Reader.mPassCount %llu",mPassCount);
   Prn::print(0,"Reader.mFailCount %llu",mFailCount);
   Prn::print(0,"Reader.mCheckSum   %llu",mCheckSum);
   Prn::print(0,"");
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::startTrial()
{
   mMarkerRead.startTrial(gParms.mXLimit);

}
void Reader::finishTrial()
{
   mMarkerRead.finishTrial();

   mMeanTimeRead = mMarkerRead.mStatistics.mMean;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType1(int aNumReads)
{
   LFBackoff tDelayB(gParms.mDelayB1, gParms.mDelayB2);

   for (int i = 0; i < aNumReads; i++)
   {
      bool tPass;
      int tCount;

      mMarkerRead.doStart();
      tPass = gShare.mSRSWIntQueue.tryRead(&tCount);
      mMarkerRead.doStop();

      tDelayB.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType2(int aNumReads)
{
   LFBackoff tDelayB(gParms.mDelayB1, gParms.mDelayB2);

   for (int i = 0; i < aNumReads; i++)
   {
      bool tPass;
      int tCount;

      mMarkerRead.doStart();

      Class1A* tObject = gShare.mSRSWObjectQueue.startRead();
      if (tObject)
      {
         tCount = tObject->mCode1;
         gShare.mSRSWObjectQueue.finishRead();
      }

      mMarkerRead.doStop();
      tPass = tObject != 0;

      tDelayB.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType3(int aNumReads)
{
   LFBackoff tDelayB(gParms.mDelayB1, gParms.mDelayB2);

   for (int i = 0; i < aNumReads; i++)
   {
      bool tPass = false;
      int tCount;

      mMarkerRead.doStart();

      Class1A* tObject = (Class1A*)gShare.mLCPointerQueue.tryRead();

      if (tObject)
      {
         tCount = tObject->mCode1;
         delete tObject;
         mMarkerRead.doStop();
         tPass = true;
      }

      tDelayB.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType4(int aNumReads)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType5(int aNumReads)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType6(int aNumReads)
{
   LFBackoff tDelayB(gParms.mDelayB1,gParms.mDelayB2);

   for (int i = 0; i < aNumReads; i++)
   {
      bool tPass;
      int tCount;

      mMarkerRead.doStart();
      tPass = gShare.mSRSWIntQueue.tryRead(&tCount);
      mMarkerRead.doStop();

      tDelayB.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType7(int aNumReads)
{
   LFBackoff tDelayB(gParms.mDelayB1,gParms.mDelayB2);

   for (int i = 0; i < aNumReads; i++)
   {
      bool tPass;
      int tCount;
      int tIndex;

      mMarkerRead.doStart();

      Class1A* tObject = (Class1A*)gShare.mLCObjectQueue.startRead(&tIndex);
      if (tObject)
      {
         tCount = tObject->mCode1;
         gShare.mLCObjectQueue.finishRead(tIndex);
         mMarkerRead.doStop();
      }

      tPass = tObject!=0;

      tDelayB.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         mCount++;
         mFailCount++;
      }
   }
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType8(int aNumReads)
{
   LFBackoff tDelayB(gParms.mDelayB1, gParms.mDelayB2);

   for (int i = 0; i < aNumReads; i++)
   {
      bool tPass;
      int tCount;

      mMarkerRead.doStart();
      tPass = gShare.mLCIntQueue.tryRead(&tCount);
      mMarkerRead.doStop();

      tDelayB.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType9(int aNumReads)
{
   LFBackoff tDelayB(gParms.mDelayB1, gParms.mDelayB2);

   for (int i = 0; i < aNumReads; i++)
   {
      bool tPass = false;
      int tCount;
      
      MyBlockX* tObject = (MyBlockX*)gShare.mLCPointerQueue.tryRead();
      tDelayB.delay();

      if (tObject)
      {
         tCount = tObject->mCode1;
         mMarkerRead.doStart();
         delete tObject;
         mMarkerRead.doStop();
         tPass = true;
      }

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::readType10(int aNumReads)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::read(int aNumReads)
{
   switch (gShare.mType)
   {
   case 1: readType1(aNumReads); break;
   case 2: readType2(aNumReads); break;
   case 3: readType3(aNumReads); break;
   case 4: readType4(aNumReads); break;
   case 5: readType5(aNumReads); break;
   case 6: readType6(aNumReads); break;
   case 7: readType7(aNumReads); break;
   case 8: readType8(aNumReads); break;
   case 9: readType9(aNumReads); break;
   case 10: readType10(aNumReads); break;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType1()
{
   while (true)
   {
      int tCount;
      bool tPass;
      tPass = gShare.mSRSWIntQueue.tryRead(&tCount);
      if (!tPass) break;

      mCount++;
      mPassCount++;
      mCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType2()
{
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType3()
{
   while (true)
   {
      Class1A* tObject = (Class1A*)gShare.mLCPointerQueue.tryRead();
      if (!tObject) break;

      int tCount = tObject->mCode1;
      delete tObject;

      mCount++;
      mPassCount++;
      mCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType4()
{
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType5()
{
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType6()
{
   while(true)
   {
      int tCount;
      bool tPass;
      tPass = gShare.mSRSWIntQueue.tryRead(&tCount);
      if (!tPass) break;

      mCount++;
      mPassCount++;
      mCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType7()
{
   int tCount;
   int tIndex;

   while(true)
   {

      Class1A* tObject = (Class1A*)gShare.mLCObjectQueue.startRead(&tIndex);
      if (!tObject) break;

      tCount = tObject->mCode1;
      gShare.mLCObjectQueue.finishRead(tIndex);

      mCount++;
      mPassCount++;
      mCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType8()
{
   while (true)
   {
      int tCount;
      bool tPass;
      tPass = gShare.mLCIntQueue.tryRead(&tCount);
      if (!tPass) break;

      mCount++;
      mPassCount++;
      mCheckSum += tCount;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType9()
{
   while (true)
   {
      MyBlockX* tObject =  (MyBlockX*)gShare.mLCPointerQueue.tryRead();
      if (!tObject) break;

      int tCount = tObject->mCode1;
      delete tObject;

      mCount++;
      mPassCount++;
      mCheckSum += tCount;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flushType10()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Reader::flush()
{
   switch (gShare.mType)
   {
   case 1: flushType1(); break;
   case 2: flushType2(); break;
   case 3: flushType3(); break;
   case 4: flushType4(); break;
   case 5: flushType5(); break;
   case 6: flushType6(); break;
   case 7: flushType7(); break;
   case 8: flushType8(); break;
   case 9: flushType9(); break;
   case 10: flushType10(); break;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace