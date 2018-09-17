#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "ccBlockPool.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MyBlockC
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   int  mIdentifier;
   int  mCode1;
   int  mCode2;
   int  mCode3;
   int  mCode4;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Allocate or deallocate a block from the block pool.
   static void* operator new      (size_t sz);
   static void  operator delete   (void* ptr);

   // Constructors.
   MyBlockC();
   MyBlockC(int aIdentifier);

   // Destructor.
  ~MyBlockC();

  //***************************************************************************
  //***************************************************************************
  //***************************************************************************
  // Methods.

   void method1();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
