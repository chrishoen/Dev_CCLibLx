/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "ccBlockPool.h"
#include "someBlockPoolIndex.h"
#include "someMyBlockX.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Allocate a block from the block pool at the given block pool index.
// Call the constructor with placement new.
// Return a pointer to the allocated block.
// Return null if the block pool is empty.

MyBlockX* MyBlockX::create()
{
   // Block pointer.
   void* tBlockPointer = 0;

   // Try to allocate a block from the block pool.
   CC::allocateBlockPoolBlock(Some::cBlockPoolIndex_MyBlockX, (void**)&tBlockPointer, 0);

   // Guard.
   if (tBlockPointer == 0) return 0;

   // Call the constructor with placement new.
   MyBlockX* tPtr = new (tBlockPointer)  MyBlockX;

   // Return the pointer to the allocated block.
   return tPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Allocate a block from the block pool at the given block pool index.
// Call the constructor with placement new.
// Return a pointer to the allocated block.
// Return null if the block pool is empty.

MyBlockX* MyBlockX::create(int aIdentifier)
{
   // Block pointer.
   void* tBlockPointer = 0;

   // Try to allocate a block from the block pool.
   CC::allocateBlockPoolBlock(Some::cBlockPoolIndex_MyBlockX, (void**)&tBlockPointer, 0);

   // Guard.
   if (tBlockPointer == 0) return 0;

   // Call the constructor with placement new.
   MyBlockX* tPtr = new (tBlockPointer)  MyBlockX(aIdentifier);

   // Return the pointer to the allocated block.
   return tPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Deallocate a block from the block pool.

void MyBlockX::operator delete(void* ptr)
{
   // Deallocate the block back to the block pool
   CC::deallocateBlockPoolBlock(ptr);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Default Constructor.

MyBlockX::MyBlockX()
{
   mIdentifier = 99;
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

MyBlockX::MyBlockX(int aIdentifier)
{
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

MyBlockX::~MyBlockX()
{
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MyBlockX::method1()
{
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace