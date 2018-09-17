/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "Parms.h"
#include "LFBackoff.h"
#include "LFFreeList.h"
#include "LFIntQueue.h"
#include "someMyBlockX.h"
#include "someShare.h"
#include "someWriter.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

Writer::Writer()
{
   initialize();
}

void Writer::initialize(unsigned aIdent)
{
   mIdent = aIdent;

   mCount     = 0;
   mPassCount = 0;
   mFailCount = 0;
   mCheckSum   = 0;
   mMeanTimeWrite  = 0.0;
   mMeanTimePop  = 0.0;
   mMeanTimePush  = 0.0;
}

void Writer::finalize()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Writer::show()
{
   Prn::print(0,"Writer.mIdent     %d",  mIdent);
   Prn::print(0,"Writer.mCount     %llu",mCount);
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Writer::writeType1(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      bool tPass;
      int tCount = mCount & 0xFFFF;

      mMarkerWrite.doStart();
      tPass = LFIntQueue::tryWrite(tCount);
      mMarkerWrite.doStop();
      tDelayA.delay();

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

void Writer::writeType2(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      bool tPass;
      int tCount = mCount & 0xFFFF;

      Class1A* tObject = new Class1A;
      tObject->mCode1 = tCount;

      mMarkerWrite.doStart();
      tPass = gShare.mLFPointerQueue.writePtr(tObject);
      mMarkerWrite.doStop();
      tDelayA.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         delete tObject;
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Writer::writeType3(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      bool tPass = false;
      int tCount = mCount & 0xFFFF;

      mMarkerWrite.doStart();

      int tIndex;
      void* tPacket = gShare.mLFObjectQueue.startWrite(&tIndex);
      if (tPacket)
      {
         Class1A* tObject = new(tPacket) Class1A;
         tObject->mCode1 = tCount;
         tDelayA.delay();
         gShare.mLFObjectQueue.finishWrite(tIndex);
         tPass=true;
      }

      mMarkerWrite.doStop();
      tDelayA.delay();

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

void Writer::writeType4(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      MyBlockX* tObject = 0;
      bool tPass = false;
      int tCount = mCount & 0xFFFF;

      mMarkerWrite.doStart();
      tObject = new MyBlockX();
      mMarkerWrite.doStop();

      if (tObject)
      {
         tObject->mCode1 = tCount;
         tPass = gShare.mLFValueQueue.tryWrite(tObject);
      }
      else
      {
         tPass=false;
      }

      tDelayA.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         if (tObject)
         {
            delete tObject;
         }
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Writer::writeType5(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      bool tPass;
      int tCount = mCount & 0xFFFF;

      mMarkerWrite.doStart();
      tPass = gShare.mLFIntQueue.tryWrite(tCount);
      mMarkerWrite.doStop();

      tDelayA.delay();

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

void Writer::writeType6(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      bool tPass;
      int tCount = mCount & 0xFFFF;

      mMarkerWrite.doStart();
      tPass = gShare.mSRSWIntQueue.tryWrite(tCount);
      mMarkerWrite.doStop();

      tDelayA.delay();

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

void Writer::writeType7(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      bool tPass = false;
      int tCount = mCount & 0xFFFF;

      mMarkerWrite.doStart();

      int tIndex;
      void* tPacket = gShare.mLMObjectQueue.startWrite(&tIndex);
      if (tPacket)
      {
         Class1A* tObject = new(tPacket) Class1A;
         tObject->mCode1 = tCount;
         tDelayA.delay();
         gShare.mLMObjectQueue.finishWrite(tIndex);
         tPass=true;
      }

      mMarkerWrite.doStop();
      tDelayA.delay();

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

void Writer::writeType8(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1, gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      bool tPass;
      int tCount = mCount & 0xFFFF;

      mMarkerWrite.doStart();
      tPass = gShare.mLMIntQueue.tryWrite(tCount);
      mMarkerWrite.doStop();

      tDelayA.delay();

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

void Writer::writeType9(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1, gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      MyBlockX* tObject = 0;
      bool tPass = false;
      int tCount = mCount & 0xFFFF;

      mMarkerWrite.doStart();
      tObject = MyBlockX::create();
      mMarkerWrite.doStop();

      if (tObject)
      {
         tObject->mCode1 = tCount;
         tPass = gShare.mLMPointerQueue.tryWrite(tObject);
      }
      else
      {
         tPass = false;
      }

      tDelayA.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         if (tObject)
         {
            delete tObject;
         }
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Writer::writeType10(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1, gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      Class1A* tObject = 0;
      bool tPass = false;
      int tCount = mCount & 0xFFFF;

      mMarkerWrite.doStart();
      tObject = new Class1A();
      mMarkerWrite.doStop();

      if (tObject)
      {
         tObject->mCode1 = tCount;
         tPass = gShare.mLMPointerQueue.tryWrite(tObject);
      }
      else
      {
         tPass = false;
      }

      tDelayA.delay();

      if (tPass)
      {
         mCount++;
         mPassCount++;
         mCheckSum += tCount;
      }
      else
      {
         if (tObject)
         {
            delete tObject;
         }
         mCount++;
         mFailCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Writer::startTrial()
{
   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerPop.startTrial(gParms.mXLimit);
   mMarkerPush.startTrial(gParms.mXLimit);
}

void Writer::finishTrial()
{
   mMarkerWrite.finishTrial();
   mMarkerPop.finishTrial();
   mMarkerPush.finishTrial();

   mMeanTimeWrite = mMarkerWrite.mStatistics.mMean;
   mMeanTimePop = mMarkerPop.mStatistics.mMean;
   mMeanTimePush = mMarkerPush.mStatistics.mMean;
}

void Writer::write(int aNumWrites)
{
   switch (gShare.mType)
   {
   case 1: writeType1(aNumWrites); break;
   case 2: writeType2(aNumWrites); break;
   case 3: writeType3(aNumWrites); break;
   case 4: writeType4(aNumWrites); break;
   case 5: writeType5(aNumWrites); break;
   case 6: writeType6(aNumWrites); break;
   case 7: writeType7(aNumWrites); break;
   case 8: writeType8(aNumWrites); break;
   case 9: writeType9(aNumWrites); break;
   case 10: writeType10(aNumWrites); break;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace