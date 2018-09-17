#ifndef _SOMEMYBLOCKR_H_
#define _SOMEMYBLOCKR_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <new>
#include "ccBlockPool.h"
#include "someMyBlockA.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MyBlockR : public Some::MyBlockA
{
public:

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   // Memory management, constructors and destructors

   // Allocate or deallocate a block from the block pool.
   static void* operator new      (size_t sz);
   static void  operator delete   (void* ptr);

   // Constructors.
   MyBlockR();
   MyBlockR(int aIdentifier);
   // Destructor.
  ~MyBlockR();

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   // Members

   int  mIdentifier;
   int  mCode1;
   int  mCode2;
   int  mCode3;
   int  mCode4;

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   // Methods

   void method1();
};

//******************************************************************************
}//namespace

#endif

