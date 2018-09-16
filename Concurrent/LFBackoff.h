#ifndef _LFBACKOFF_H_
#define _LFBACKOFF_H_

#include <math.h>
#include "my_functions.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

class LFBackoff
{
public:

   //---------------------------------------------------------------------------
   // Members

   int  mInitial1;
   int  mInitial2;
   int  mDelay1;
   int  mDelay2;
   int  mCount;
   bool mEnable;

   //---------------------------------------------------------------------------
   // Convert from microseconds to counts

   inline static int convertFromUsec(double aDelay)
   {
#ifdef _DEBUG
      int tN = lround(385.79*aDelay - 38.827);
#else
      int tN = lround(513.11*aDelay - 8.46);
#endif
      if (tN<0) tN=0;
      return tN;
   }

   //---------------------------------------------------------------------------
   // Constructor

   inline LFBackoff()
   {
      mInitial1=0;
      mInitial2=0;
      mDelay1=0;
      mDelay2=0;
      mCount=0;
      mEnable=false;
   }

   inline LFBackoff(int aDelay1,int aDelay2)
   {
      mInitial1=aDelay1;
      mInitial2=aDelay2;
      mDelay1=mInitial1;
      mDelay2=mInitial2;
      mCount=0;
      mEnable = mInitial2!=0;
   }

   inline LFBackoff(double aDelayUsec1, double aDelayUsec2)
   {
      mInitial1 = convertFromUsec(aDelayUsec1);
      mInitial2 = convertFromUsec(aDelayUsec2);
      mDelay1=mInitial1;
      mDelay2=mInitial2;
      mCount=0;
      mEnable = mInitial2!=0;
   }

   //---------------------------------------------------------------------------
   // Set delays

   inline void setDelay(int aDelay1, int aDelay2)
   {
      mInitial1=aDelay1;
      mInitial2=aDelay2;
      mDelay1=mInitial1;
      mDelay2=mInitial2;
      mCount=0;
      mEnable = mInitial2!=0;
   }

   inline void setDelay(double aDelayUsec1, double aDelayUsec2)
   {
      mInitial1 = convertFromUsec(aDelayUsec1);
      mInitial2 = convertFromUsec(aDelayUsec2);
      mDelay1=mInitial1;
      mDelay2=mInitial2;
      mCount=0;
      mEnable = mInitial2!=0;
   }

   //---------------------------------------------------------------------------
   // Reset

   inline void reset()
   {
      mDelay1=mInitial1;
      mDelay2=mInitial2;
      mCount=0;
      mEnable = mInitial2!=0;
   }

   //---------------------------------------------------------------------------
   // Exponential backoff

   inline void expBackoff()
   {
      if (!mEnable)return;

      volatile int tDummy=0;
      int tLoop = my_irand(mDelay1,mDelay2);
      for (int i=0;i<tLoop;i++) tDummy++;

      if (++mCount < 5)
      {
         mDelay1 *= 2;
         mDelay2 *= 2;
      }
   }

   //---------------------------------------------------------------------------
   // Linear backoff

   inline void linearBackoff()
   {
      if (!mEnable)return;

      volatile int tDummy=0;
      int tLoop = my_irand(mDelay1,mDelay2);
      for (int i=0;i<tLoop;i++) tDummy++;

      if (++mCount < 5)
      {
         mDelay1 += mInitial1;
         mDelay2 += mInitial2;
      }
   }

   //---------------------------------------------------------------------------
   // Constant backoff

   inline void delay()
   {
      if (!mEnable)return;

      volatile int tDummy=0;
      int tLoop = my_irand(mDelay1,mDelay2);
      for (int i=0;i<tLoop;i++) tDummy++;
   }
};


#endif

