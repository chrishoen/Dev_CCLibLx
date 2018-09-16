#pragma once

/*==============================================================================
==============================================================================*/

#include <stdio.h>
#include "risTimeMarker.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class Reader
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   unsigned long long mCount;
   unsigned long long mPassCount;
   unsigned long long mFailCount;
   unsigned long long mCheckSum;

   Ris::TrialTimeMarker mMarkerRead;
   double               mMeanTimeRead;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   Reader();

   void initialize();
   void finalize();
   void show();

   void startTrial();
   void finishTrial();

   void readType1(int aNumReads);
   void readType2(int aNumReads);
   void readType3(int aNumReads);
   void readType4(int aNumReads);
   void readType5(int aNumReads);
   void readType6(int aNumReads);
   void readType7(int aNumReads);
   void readType8(int aNumReads);
   void readType9(int aNumReads);
   void readType10(int aNumReads);
   void read(int aNumReads);

   void flushType1();
   void flushType2();
   void flushType3();
   void flushType4();
   void flushType5();
   void flushType6();
   void flushType7();
   void flushType8();
   void flushType9();
   void flushType10();
   void flush();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


