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
#include "someClass1.h"
#include "someShare.h"
#include "someWriterReader.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

WriterReader::WriterReader()
{
   initialize();
}

void WriterReader::initialize(unsigned aIdent)
{
   mIdent = aIdent;
   mCount = 0;

   mWriteCount     = 0;
   mWritePassCount = 0;
   mWriteFailCount = 0;
   mWriteCheckSum   = 0;
   mReadCount     = 0;
   mReadPassCount = 0;
   mReadFailCount = 0;
   mReadCheckSum   = 0;

   mMeanTimeWrite  = 0.0;
   mMeanTimeRead  = 0.0;
}

void WriterReader::finalize()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::show()
{
   Prn::print(0,"WriterReader.mIdent      %d",  mIdent);
   Prn::print(0,"WriterReader.mWriteCount %llu",mWriteCount);
   Prn::print(0,"WriterReader.mReadCount  %llu",mReadCount);
   Prn::print(0,"");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType1()
{
   if (mIdent == 0)
   {
      int tListSize = gParms.mNumElements;
      for (int i = 0; i < tListSize / 2; i++)
      {
         ++mCount &= 0xFFFF;
         LFIntQueue::tryWrite(mCount);

         mWriteCount++;
         mWritePassCount++;
         mWriteCheckSum += mCount;
      }
   }

   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType2()
{
   if (mIdent == 0)
   {
      int tListSize = gParms.mNumElements;
      for (int i = 0; i < tListSize / 2; i++)
      {
         ++mCount &= 0xFFFF;

         Class1A* tObject = new Class1A;
         tObject->mCode1 = mCount;
         gShare.mLFPointerQueue.writePtr(tObject);

         mWriteCount++;
         mWritePassCount++;
         mWriteCheckSum += mCount;
      }
   }

   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType3()
{
   if (mIdent == 0)
   {
      int tListSize = gParms.mNumElements;
      for (int i = 0; i < tListSize / 2; i++)
      {
         ++mCount &= 0xFFFF;

         int tIndex;
         void* tPacket = gShare.mLFObjectQueue.startWrite(&tIndex);
         if (tPacket)
         {
            Class1A* tObject = new(tPacket) Class1A;
            tObject->mCode1 = mCount;
            gShare.mLFObjectQueue.finishWrite(tIndex);
         }

         mWriteCount++;
         mWritePassCount++;
         mWriteCheckSum += mCount;
      }
   }

   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType4()
{
   if (mIdent == 0)
   {
      int tListSize = gParms.mNumElements;
      for (int i = 0; i < tListSize / 2; i++)
      {
         ++mCount &= 0xFFFF;

         MyBlockX* tObject = new MyBlockX();
         tObject->mCode1 = mCount;
         gShare.mLFValueQueue.tryWrite(tObject);

         mWriteCount++;
         mWritePassCount++;
         mWriteCheckSum += mCount;
      }
   }

   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType5()
{
   if (mIdent == 0)
   {
      int tListSize = gParms.mNumElements;
      for (int i = 0; i < tListSize / 2; i++)
      {
         ++mCount &= 0xFFFF;

         gShare.mLFIntQueue.tryWrite(mCount);

         mWriteCount++;
         mWritePassCount++;
         mWriteCheckSum += mCount;
      }
   }

   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType6()
{
   if (mIdent == 0)
   {
      int tListSize = gParms.mNumElements;
      for (int i = 0; i < tListSize / 2; i++)
      {
         ++mCount &= 0xFFFF;

         gShare.mSRSWIntQueue.tryWrite(mCount);

         mWriteCount++;
         mWritePassCount++;
         mWriteCheckSum += mCount;
      }
   }

   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType11()
{
   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType12()
{
   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType21()
{
   if (mIdent == 0)
   {
      int tListSize = gParms.mNumElements;
      for (int i = 0; i < tListSize / 2; i++)
      {
         ++mCount &= 0xFFFF;

         Class1A* tObject = new Class1A();
         tObject->mCode1 = mCount;
         gShare.mLFValueQueue.tryWrite(tObject);

         mWriteCount++;
         mWritePassCount++;
         mWriteCheckSum += mCount;
      }
   }

   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrialType22()
{
   if (mIdent == 0)
   {
      int tListSize = gParms.mNumElements;
      for (int i = 0; i < tListSize / 2; i++)
      {
         ++mCount &= 0xFFFF;

         MyBlockX* tObject = new MyBlockX();
         tObject->mCode1 = mCount;
         gShare.mLFValueQueue.tryWrite(tObject);

         mWriteCount++;
         mWritePassCount++;
         mWriteCheckSum += mCount;
      }
   }

   mMarkerWrite.startTrial(gParms.mXLimit);
   mMarkerRead.startTrial(gParms.mXLimit);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::startTrial()
{
   switch (gShare.mType)
   {
   case 1:  startTrialType1 ();  break;
   case 2:  startTrialType2 ();  break;
   case 3:  startTrialType3 ();  break;
   case 4:  startTrialType4 ();  break;
   case 5:  startTrialType5 ();  break;
   case 6:  startTrialType6 ();  break;
   case 11: startTrialType11 (); break;
   case 12: startTrialType12 (); break;
   case 21: startTrialType21 (); break;
   case 22: startTrialType22 (); break;
   }
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::finishTrial()
{
   mMarkerWrite.finishTrial();
   mMarkerRead.finishTrial();

   mMeanTimeWrite = mMarkerWrite.mStatistics.mMean;
   mMeanTimeRead = mMarkerRead.mStatistics.mMean;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType1(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      // Write
      if (my_randflag(0.5))
      {
         ++mCount &= 0xFFFF;
         bool tPass;

         mMarkerWrite.doStart();
         tPass = LFIntQueue::tryWrite(mCount);
         mMarkerWrite.doStop();
         tDelayA.delay();

         if (tPass)
         {
            mWriteCount++;
            mWritePassCount++;
            mWriteCheckSum += mCount;
         }
         else
         {
            mWriteCount++;
            mWriteFailCount++;
         }
      }
      // Read
      else
      {
         bool tPass;
         int tCount;
         mMarkerRead.doStart();
         tPass = LFIntQueue::tryRead(&tCount);
         mMarkerRead.doStop();
         tDelayA.delay();

         if (tPass)
         {
            mReadCount++;
            mReadPassCount++;
            mReadCheckSum += tCount;
         }
         else
         {
            mReadCount++;
            mReadFailCount++;
         }
      }
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType2(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      // Write
      if (my_randflag(0.5))
      {
         ++mCount &= 0xFFFF;

         Class1A* tObject = new Class1A;
         tObject->mCode1 = mCount;

         mMarkerWrite.doStart();
         bool tPass = gShare.mLFPointerQueue.writePtr(tObject);
         mMarkerWrite.doStop();
         tDelayA.delay();

         if (tPass)
         {
            mWriteCount++;
            mWritePassCount++;
            mWriteCheckSum += mCount;
         }
         else
         {
            delete tObject;
            mWriteCount++;
            mWriteFailCount++;
         }
      }
      // Read
      else
      {
         bool tPass;
         int tCount;

         mMarkerRead.doStart();
         Class1A* tObject = (Class1A*)gShare.mLFPointerQueue.readPtr();
         mMarkerRead.doStop();
         tDelayA.delay();

         tPass = tObject!=0;
         if (tObject)
         {
            tCount = tObject->mCode1;
            delete tObject;
         }

         if (tPass)
         {
            mReadCount++;
            mReadPassCount++;
            mReadCheckSum += tCount;
         }
         else
         {
            mReadCount++;
            mReadFailCount++;
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType3(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      // Write
      if (my_randflag(0.5))
      {
         ++mCount &= 0xFFFF;

         mMarkerWrite.doStart();

         int tIndex;
         void* tPacket = gShare.mLFObjectQueue.startWrite(&tIndex);
         if (tPacket)
         {
            Class1A* tObject = new(tPacket) Class1A;
            tObject->mCode1 = mCount;
            gShare.mLFObjectQueue.finishWrite(tIndex);
         }

         mMarkerWrite.doStop();
         tDelayA.delay();

         if (tPacket)
         {
            mWriteCount++;
            mWritePassCount++;
            mWriteCheckSum += mCount;
         }
         else
         {
            mWriteCount++;
            mWriteFailCount++;
         }
      }
      // Read
      else
      {
         bool tPass = false;
         int tIndex;
         int tCount;

         mMarkerRead.doStart();
         Class1A* tObject = (Class1A*)gShare.mLFObjectQueue.startRead(&tIndex);
         if (tObject)
         {
            tCount = tObject->mCode1;
            gShare.mLFObjectQueue.finishRead(tIndex);
            tPass = true;
         }
         mMarkerRead.doStop();
         tDelayA.delay();

         if (tPass)
         {
            mReadCount++;
            mReadPassCount++;
            mReadCheckSum += tCount;
         }
         else
         {
            mReadCount++;
            mReadFailCount++;
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType4(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      // Write
      if (my_randflag(0.5))
      {
         ++mCount &= 0xFFFF;

         MyBlockX* tObject = 0;
         bool tPass = false;

         mMarkerWrite.doStart();
         tObject = new MyBlockX();
         mMarkerWrite.doStop();

         if (tObject)
         {
            tObject->mCode1 = mCount;
            tPass = gShare.mLFValueQueue.tryWrite(tObject);

            if (tPass)
            {
               mWriteCount++;
               mWritePassCount++;
               mWriteCheckSum += mCount;
            }
            else
            {
               delete tObject;
               mWriteCount++;
               mWriteFailCount++;
            }
         }
         tDelayA.delay();
      }
      // Read
      else
      {
         bool tPass;
         int tCount;

         MyBlockX* tObject = 0;
         tPass = gShare.mLFValueQueue.tryRead((void**)&tObject);
         tDelayA.delay();

         if (tPass)
         {
            tCount = tObject->mCode1;
            mMarkerRead.doStart();
            delete tObject;
            mMarkerRead.doStop();
         }

         if (tPass)
         {
            mReadCount++;
            mReadPassCount++;
            mReadCheckSum += tCount;
         }
         else
         {
            mReadCount++;
            mReadFailCount++;
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType5(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      // Write
      if (my_randflag(0.5))
      {
         ++mCount &= 0xFFFF;

         bool tPass = false;

         mMarkerWrite.doStart();
         tPass = gShare.mLFIntQueue.tryWrite(mCount);
         mMarkerWrite.doStop();

         tDelayA.delay();

         if (tPass)
         {
            mWriteCount++;
            mWritePassCount++;
            mWriteCheckSum += mCount;
         }
         else
         {
            mWriteCount++;
            mWriteFailCount++;
         }

      }
      // Read
      else
      {
         bool tPass;
         int tCount;

         mMarkerRead.doStart();
         tPass = gShare.mLFIntQueue.tryRead(&tCount);
         mMarkerRead.doStop();

         tDelayA.delay();

         if (tPass)
         {
            mReadCount++;
            mReadPassCount++;
            mReadCheckSum += tCount;
         }
         else
         {
            mReadCount++;
            mReadFailCount++;
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType6(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      // Write
      if (my_randflag(0.5))
      {
         ++mCount &= 0xFFFF;

         bool tPass = false;

         mMarkerWrite.doStart();
         tPass = gShare.mSRSWIntQueue.tryWrite(mCount);
         mMarkerWrite.doStop();

         tDelayA.delay();

         if (tPass)
         {
            mWriteCount++;
            mWritePassCount++;
            mWriteCheckSum += mCount;
         }
         else
         {
            mWriteCount++;
            mWriteFailCount++;
         }

      }
      // Read
      else
      {
         bool tPass;
         int tCount;

         mMarkerRead.doStart();
         tPass = gShare.mSRSWIntQueue.tryRead(&tCount);
         mMarkerRead.doStop();

         tDelayA.delay();

         if (tPass)
         {
            mReadCount++;
            mReadPassCount++;
            mReadCheckSum += tCount;
         }
         else
         {
            mReadCount++;
            mReadFailCount++;
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType11(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      Class1A* tObject=0;

      mMarkerWrite.doStart();
      tObject = new Class1A;
      mMarkerWrite.doStop();
      tDelayA.delay();

      mWriteCount++;
      mWritePassCount++;

      gShare.putDummy(tObject);

      mMarkerRead.doStart();
      delete tObject;
      mMarkerRead.doStop();
      tDelayA.delay();

      mReadCount++;
      mReadPassCount++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType12(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      MyBlockX* tObject=0;

      mMarkerWrite.doStart();
      tObject = new MyBlockX;
      mMarkerWrite.doStop();
      tDelayA.delay();

      mWriteCount++;
      mWritePassCount++;

      gShare.putDummy(tObject);

      mMarkerRead.doStart();
      delete tObject;
      mMarkerRead.doStop();
      tDelayA.delay();

      mReadCount++;
      mReadPassCount++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType21(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      // Write
      if (my_randflag(0.5))
      {
         ++mCount &= 0xFFFF;

         Class1A* tObject = 0;
         bool tPass = false;

         mMarkerWrite.doStart();
         tObject = new Class1A();
         mMarkerWrite.doStop();

         if (tObject)
         {
            tObject->mCode1 = mCount;
            tPass = gShare.mLFValueQueue.tryWrite(tObject);

            if (tPass)
            {
               mWriteCount++;
               mWritePassCount++;
               mWriteCheckSum += mCount;
            }
            else
            {
               delete tObject;
               mWriteCount++;
               mWriteFailCount++;
            }
         }
         tDelayA.delay();
      }
      // Read
      else
      {
         bool tPass;
         int tCount;

         Class1A* tObject = 0;
         tPass = gShare.mLFValueQueue.tryRead((void**)&tObject);
         tDelayA.delay();

         if (tPass)
         {
            tCount = tObject->mCode1;
            mMarkerRead.doStart();
            delete tObject;
            mMarkerRead.doStop();
         }

         if (tPass)
         {
            mReadCount++;
            mReadPassCount++;
            mReadCheckSum += tCount;
         }
         else
         {
            mReadCount++;
            mReadFailCount++;
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writereadType22(int aNumWrites)
{
   LFBackoff tDelayA(gParms.mDelayA1,gParms.mDelayA2);

   for (int i = 0; i < aNumWrites; i++)
   {
      // Write
      if (my_randflag(0.5))
      {
         ++mCount &= 0xFFFF;

         MyBlockX* tObject = 0;
         bool tPass = false;

         mMarkerWrite.doStart();
         tObject = new MyBlockX();
         mMarkerWrite.doStop();

         if (tObject)
         {
            tObject->mCode1 = mCount;
            tPass = gShare.mLFValueQueue.tryWrite(tObject);

            if (tPass)
            {
               mWriteCount++;
               mWritePassCount++;
               mWriteCheckSum += mCount;
            }
            else
            {
               delete tObject;
               mWriteCount++;
               mWriteFailCount++;
            }
         }
         tDelayA.delay();
      }
      // Read
      else
      {
         bool tPass;
         int tCount;

         MyBlockX* tObject = 0;
         tPass = gShare.mLFValueQueue.tryRead((void**)&tObject);
         tDelayA.delay();

         if (tPass)
         {
            tCount = tObject->mCode1;
            mMarkerRead.doStart();
            delete tObject;
            mMarkerRead.doStop();
         }

         if (tPass)
         {
            mReadCount++;
            mReadPassCount++;
            mReadCheckSum += tCount;
         }
         else
         {
            mReadCount++;
            mReadFailCount++;
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::writeread(int aNumWrites)
{
   switch (gShare.mType)
   {
   case 1:  writereadType1  (aNumWrites); break;
   case 2:  writereadType2  (aNumWrites); break;
   case 3:  writereadType3  (aNumWrites); break;
   case 4:  writereadType4  (aNumWrites); break;
   case 5:  writereadType5  (aNumWrites); break;
   case 6:  writereadType6  (aNumWrites); break;
   case 11: writereadType11 (aNumWrites); break;
   case 12: writereadType12 (aNumWrites); break;
   case 21: writereadType21 (aNumWrites); break;
   case 22: writereadType22 (aNumWrites); break;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType1()
{
   while(true)
   {
      int tCount;
      if (!LFIntQueue::tryRead(&tCount)) break;
      mReadCount++;
      mReadPassCount++;
      mReadCheckSum += tCount;
   }
}
   

//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType2()
{
   while(true)
   {
      int tCount;
      Class1A* tObject = (Class1A*)gShare.mLFPointerQueue.readPtr();
      if (tObject==0) break;
      tCount = tObject->mCode1;
      delete tObject;
      mReadCount++;
      mReadPassCount++;
      mReadCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType3()
{
   int tCount;
   int tIndex;

   while(true)
   {

      Class1A* tObject = (Class1A*)gShare.mLFObjectQueue.startRead(&tIndex);
      if (!tObject) break;

      tCount = tObject->mCode1;
      gShare.mLFObjectQueue.finishRead(tIndex);

      mReadCount++;
      mReadPassCount++;
      mReadCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType4()
{
   while(true)
   {
      int tCount;
      bool tPass;
      MyBlockX* tObject = 0;
      tPass = gShare.mLFValueQueue.tryRead((void**)&tObject);
      if (!tPass) break;
      tCount = tObject->mCode1;
      delete tObject;
      mReadCount++;
      mReadPassCount++;
      mReadCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType5()
{
   while(true)
   {
      int tCount;
      bool tPass;
      tPass = gShare.mLFIntQueue.tryRead(&tCount);
      if (!tPass) break;
      mReadCount++;
      mReadPassCount++;
      mReadCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType6()
{
   while(true)
   {
      int tCount;
      bool tPass;
      tPass = gShare.mSRSWIntQueue.tryRead(&tCount);
      if (!tPass) break;
      mReadCount++;
      mReadPassCount++;
      mReadCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType11()
{
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType12()
{
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType21()
{
   while(true)
   {
      int tCount;
      bool tPass;
      Class1A* tObject = 0;
      tPass = gShare.mLFValueQueue.tryRead((void**)&tObject);
      if (!tPass) break;
      tCount = tObject->mCode1;
      delete tObject;
      mReadCount++;
      mReadPassCount++;
      mReadCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flushType22()
{
   while(true)
   {
      int tCount;
      bool tPass;
      MyBlockX* tObject = 0;
      tPass = gShare.mLFValueQueue.tryRead((void**)&tObject);
      if (!tPass) break;
      tCount = tObject->mCode1;
      delete tObject;
      mReadCount++;
      mReadPassCount++;
      mReadCheckSum += tCount;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************

void WriterReader::flush()
{
   switch (gShare.mType)
   {
   case 1:  flushType1  (); break;
   case 2:  flushType2  (); break;
   case 3:  flushType3  (); break;
   case 4:  flushType4  (); break;
   case 5:  flushType5  (); break;
   case 6:  flushType6  (); break;
   case 11: flushType11 (); break;
   case 12: flushType12 (); break;
   case 21: flushType21 (); break;
   case 22: flushType22 (); break;
   }
}
   
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace