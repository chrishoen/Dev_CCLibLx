/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "ccBlockPool.h"
#include "someBlockPoolIndex.h"
#include "someMyBlockR.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Allocate a block from the block pool at the given block pool index.
// Return a pointer to the allocated block.
// Return null if the block pool is empty.

void* MyBlockR::operator new(size_t sz)
{
   Prn::print(0, "MyBlockR::new   %d",(int)sz);

   // Block pointer.
   void* tBlockPointer = 0;

   // Try to allocate a block from the block pool.
   CC::allocateBlockPoolBlock(Some::cBlockPoolIndex_MyBlockR, (void**)&tBlockPointer, 0);

   // Return the pointer to the allocated block.
   // Return null if the block pool is empty.
   return tBlockPointer;
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Deallocate a block from the block pool.

void MyBlockR::operator delete(void* ptr)
{
   Prn::print(0, "MyBlockR::delete");
   // Deallocate the block back to the block pool
   CC::deallocateBlockPoolBlock(ptr);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Default Constructor.

MyBlockR::MyBlockR()
{
   Prn::print(0, "MyBlockR::constructor");

   mIdentifier = 0;
   mCode1=101;
   mCode2=102;
   mCode3=103;
   mCode4=104;

   mBlockHandle.set(this);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Constructor, it is called by create after allocation of a new block.

MyBlockR::MyBlockR(int aIdentifier)
{
   Prn::print(0, "MyBlockR::constructor %d",aIdentifier);

   mIdentifier = aIdentifier;
   mCode1=101;
   mCode2=102;
   mCode3=103;
   mCode4=104;

   mBlockHandle.set(this);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

MyBlockR::~MyBlockR()
{
   Prn::print(0, "MyBlockR::destuctor   %d",mIdentifier);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MyBlockR::method1()
{
   Prn::print(0, "MyBlockR::method1     %d",mIdentifier);
}

}//namespace