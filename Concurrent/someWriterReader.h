#pragma once

/*==============================================================================
==============================================================================*/

#include "someMessage.h"
#include "risTimeMarker.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************


namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class WriterReader
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   unsigned short mIdent;
   int mCount;

   unsigned long long   mWriteCount;
   unsigned long long   mWritePassCount;
   unsigned long long   mWriteFailCount;
   unsigned long long   mWriteCheckSum;

   unsigned long long   mReadCount;
   unsigned long long   mReadPassCount;
   unsigned long long   mReadFailCount;
   unsigned long long   mReadCheckSum;

   Ris::TrialTimeMarker mMarkerWrite;
   double               mMeanTimeWrite;
   Ris::TrialTimeMarker mMarkerRead;
   double               mMeanTimeRead;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   WriterReader();
   void initialize(unsigned aIdent=0);
   void finalize();
   void show();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   void startTrialType1 ();
   void startTrialType2 ();
   void startTrialType3 ();
   void startTrialType4 ();
   void startTrialType5 ();
   void startTrialType6 ();
   void startTrialType11();
   void startTrialType12();
   void startTrialType21();
   void startTrialType22();
   void startTrial      ();
   void finishTrial();

   void writereadType1  (int aNumWrites);
   void writereadType2  (int aNumWrites);
   void writereadType3  (int aNumWrites);
   void writereadType4  (int aNumWrites);
   void writereadType5  (int aNumWrites);
   void writereadType6  (int aNumWrites);
   void writereadType11 (int aNumWrites);
   void writereadType12 (int aNumWrites);
   void writereadType21 (int aNumWrites);
   void writereadType22 (int aNumWrites);
   void writeread       (int aNumWrites);

   void flushType1  ();
   void flushType2  ();
   void flushType3  ();
   void flushType4  ();
   void flushType5  ();
   void flushType6  ();
   void flushType11 ();
   void flushType12 ();
   void flushType21 ();
   void flushType22 ();
   void flush       ();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

