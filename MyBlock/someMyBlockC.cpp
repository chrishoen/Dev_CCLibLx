/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "ccBlockPool.h"
#include "someBlockPoolIndex.h"
#include "someMyBlockC.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Allocate a block from the block pool at the given block pool index.
// Return a pointer to the allocated block.
// Return null if the block pool is empty.

void* MyBlockC::operator new(size_t sz)
{
   Prn::print(0, "MyBlockC::new   %d",(int)sz);

   // Block pointer.
   void* tBlockPointer = 0;

   // Try to allocate a block from the block pool.
   CC::allocateBlockPoolBlock(Some::cBlockPoolIndex_MyBlockC, (void**)&tBlockPointer, 0);

   // Return the pointer to the allocated block.
   // Return null if the block pool is empty.
   return tBlockPointer;
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Deallocate a block from the block pool.

void MyBlockC::operator delete(void* ptr)
{
   Prn::print(0, "MyBlockC::delete");
   // Deallocate the block back to the block pool
   CC::deallocateBlockPoolBlock(ptr);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Default Constructor.

MyBlockC::MyBlockC()
{
   Prn::print(0, "MyBlockC::constructor");

   mIdentifier = 0;
   mCode1=101;
   mCode2=102;
   mCode3=103;
   mCode4=104;

// mBlockHandle.set(this);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Constructor, it is called by create after allocation of a new block.

MyBlockC::MyBlockC(int aIdentifier)
{
   Prn::print(0, "MyBlockC::constructor %d",aIdentifier);

   mIdentifier = aIdentifier;
   mCode1=101;
   mCode2=102;
   mCode3=103;
   mCode4=104;

// mBlockHandle.set(this);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

MyBlockC::~MyBlockC()
{
   Prn::print(0, "MyBlockC::destuctor   %d",mIdentifier);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MyBlockC::method1()
{
   Prn::print(0, "MyBlockC::method1     %d",mIdentifier);
}

}//namespace