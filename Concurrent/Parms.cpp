//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _PARMS_CPP_
#include "Parms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Parms::Parms()
{
   reset();
}

void Parms::reset()
{
   BaseClass::reset();
   strcpy(BaseClass::mDefaultFileName, "Concurrent_Parms.txt");
   BaseClass::mUseSections = false;

   mMode = 1;
   mNumWriters = 1;
   mType = 1;
   mTest = 1;
   mNumElements = 1000;
   mTerminate = 0;

   mWriteLower = 900;
   mWriteUpper = 1100;
   mReadLower = 900;
   mReadUpper = 1100;
   mSleepLower = 900;
   mSleepUpper = 1100;

   mXLimit = 0.0;

   mBackQueue1 = 0.0;
   mBackQueue2 = 0.0;
   mBackList1 = 0.0;
   mBackList2 = 0.0;

   mDelayA1 = 0.0;
   mDelayA2 = 0.0;
   mDelayB1 = 0.0;
   mDelayB2 = 0.0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void Parms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Parms::show()
{
   printf("Mode          %11d\n", mMode);
   printf("Type          %11d\n", mType);
   printf("Test          %11d\n", mTest);
   printf("NumWriters    %11d\n", mNumWriters);
   printf("NumElements   %11d\n", mNumElements);
   printf("Terminate     %11d\n", mTerminate);
   printf("WriteLower    %11d\n", mWriteLower);
   printf("WriteUpper    %11d\n", mWriteUpper);
   printf("ReadLower     %11d\n", mReadLower);
   printf("ReadUpper     %11d\n", mReadUpper);
   printf("SleepLower    %11d\n", mSleepLower);
   printf("SleepUpper    %11d\n", mSleepUpper);
   printf("XLimit        %11.1f\n", mXLimit);
   printf("BackQueue1    %11.1f\n", mBackQueue1);
   printf("BackQueue2    %11.1f\n", mBackQueue2);
   printf("BackList1     %11.1f\n", mBackList1);
   printf("BackList2     %11.1f\n", mBackList2);
   printf("DelayA1       %11.1f\n", mDelayA1);
   printf("DelayA2       %11.1f\n", mDelayA2);
   printf("DelayB1       %11.1f\n", mDelayB1);
   printf("DelayB2       %11.1f\n", mDelayB2);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void Parms::execute(Ris::CmdLineCmd* aCmd)
{
// if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("Mode"))         mMode = aCmd->argInt(1);
   if (aCmd->isCmd("NumWriters"))   mNumWriters = aCmd->argInt(1);
   if (aCmd->isCmd("Type"))         mType = aCmd->argInt(1);
   if (aCmd->isCmd("Test"))         mTest = aCmd->argInt(1);
   if (aCmd->isCmd("NumElements"))  mNumElements = aCmd->argInt(1);
   if (aCmd->isCmd("Terminate"))    mTerminate = aCmd->argInt(1);
   if (aCmd->isCmd("WriteLower"))   mWriteLower = aCmd->argInt(1);
   if (aCmd->isCmd("WriteUpper"))   mWriteUpper = aCmd->argInt(1);
   if (aCmd->isCmd("ReadLower"))    mReadLower = aCmd->argInt(1);
   if (aCmd->isCmd("ReadUpper"))    mReadUpper = aCmd->argInt(1);
   if (aCmd->isCmd("SleepLower"))   mSleepLower = aCmd->argInt(1);
   if (aCmd->isCmd("SleepUpper"))   mSleepUpper = aCmd->argInt(1);
   if (aCmd->isCmd("XLimit"))       mXLimit = aCmd->argDouble(1);
   if (aCmd->isCmd("BackQueue1"))   mBackQueue1 = aCmd->argDouble(1);
   if (aCmd->isCmd("BackQueue2"))   mBackQueue2 = aCmd->argDouble(1);
   if (aCmd->isCmd("BackList1"))    mBackList1 = aCmd->argDouble(1);
   if (aCmd->isCmd("BackList2"))    mBackList2 = aCmd->argDouble(1);
   if (aCmd->isCmd("DelayA1"))      mDelayA1 = aCmd->argDouble(1);
   if (aCmd->isCmd("DelayA2"))      mDelayA2 = aCmd->argDouble(1);
   if (aCmd->isCmd("DelayB1"))      mDelayB1 = aCmd->argDouble(1);
   if (aCmd->isCmd("DelayB2"))      mDelayB2 = aCmd->argDouble(1);
}

