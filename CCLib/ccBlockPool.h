#pragma once

/*==============================================================================

A block pool is a memory management mechanism that provides a preallocated
memory region for a fixed number of blocks of a fixed size. It provides
methods to allocate and deallocate blocks from it.

Block pools are used in programs that have a bounded number of specific
objects allocated at any given time. The memory for the objects is allocated
at program initialization and allocation/deallocation of the objects
throughout the program lifetime is much faster than system heap
malloc/free operations (new/delete). To use this, the maximum number of 
blocks that can be allocated at any given time must be known apriori.

A class that uses a block pool for memory management in place of system heap
new/delete must create a specific block pool for that class at program
initialization. Creation and destruction of instances of the class must
then be done via block pool allocate/deallocate calls, new/delete cannot
be used.

There can be different types of block pools. They differ in terms of block
thread safety and concurrent access, and where block memory is located
(system heap or shared memory).

The block pools for a program are contained in a global store that contains
an array of block pool pointers. When a block pool is created, a pointer to
it is stored in the array. Accesses to the block pool (allocate or deallocate
a block, perform a handle to pointer conversion) are done via the global
block pool array.

Blocks in a block pool are accessed via a block handle. A block handle
contains a pool index and a block index. The pool index indicates which pool
a block belongs to and a block index indicates which block within the pool 
it is. Methods are provided to convert between block pointers and block
handles.

The pool index value of zero is reserved to indicate a null pool and the
block index value of zero is reserved to indicate a null block. Block
box array entry zero is a dummy block and is never used.

Classes that use block pools must contain a block handle as a member
variable. This means that the block pools are what is known as an intrusive
memory management scheme.

Block pools use block box arrays for the block memory. They allocates storage
for the blocks on the system heap or in shared memory and provides pointer 
access to the allocated blocks.

A block box contains a block header and a block body. The header is invisible
to the user and is used for things like resource counting and pointer to
handle conversions. The block body is visible to the user as a pointer to the
block.

Specific block pool classes inherit from a block pool base class.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "ccBlockPoolParms.h"

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Blocks in a block pool are accessed via a block handle. A block handle
// contains a pool index and a block index. The pool index indicates which pool
// a block belongs to and a block index indicates which block within the pool 
// it is. Methods are provided to convert between block pointers and block
// handles.
//
// The pool index value of all ones is reserved to indicate a null pool and the
// block index value of all ones is reserved to indicate a null block.
//

struct BlockHandle
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Memory pool index, specifies which memory pool a block is in.
   unsigned short mPoolIndex;

   // Memory block index, specifies which block within the memory pool.
   unsigned short mBlockIndex;

   // Invalid index value, used to mark a pool or a block as null.
   static const int  cInvalid = 0xFFFF;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructors.
   BlockHandle();
   BlockHandle(unsigned short aPoolIndex, unsigned short aBlockIndex);
   void set(unsigned short aPoolIndex, unsigned short aBlockIndex);

   // Null block handle value.
   void setNull();
   bool isNull();
   static BlockHandle null();
   static BlockHandle nullH;

   // Set a block handle from the given address of the block. This obtains the 
   // block handle by looking it up in the global block pool facility.
   void set(void* aBlockPointer);

   // Get the address of the block specified by the block handle. This obtains 
   // the handle by looking it up in the global block pool facility.
   static void* ptr(BlockHandle aBlockHandle);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Operators.

inline bool operator==(const BlockHandle& lhs, const BlockHandle& rhs)
{
   return lhs.mPoolIndex == rhs.mPoolIndex && lhs.mBlockIndex == rhs.mBlockIndex;
}

// Operator
inline bool operator!=(const BlockHandle& lhs, const BlockHandle& rhs)
{
   return lhs.mPoolIndex != rhs.mPoolIndex && lhs.mBlockIndex != rhs.mBlockIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class for classes whose storage is managed by memory pools.
// Inheriting classes contain a memory handle.

class HasBlockHandle
{
public:
   BlockHandle mBlockHandle;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Here is a set of calls that manage block pools.

// Create and initialize a block pool and place a pointer to it into the
// block pool array. It is passed a set of parameters. Store these parameters
// in persistent storage. They will be used throughout the lifetime of the 
// block pool.
void initializeBlockPoolCentral();

// Finalize the block pool facility. It is called at program
// termination. It destroys all block pools that were created.
void finalizeBlockPoolCentral();

// Create a block pool and place a pointer to it into the block
// pool array. It is passed a set of parameters.
void createBlockPool(BlockPoolParms* aParms);

// Allocate a block from a block pool. It is passes a pool index and 
// it returns a pointer to the block and a handle for the block.
// It returns true if successful, false if the block pool is empty.
bool allocateBlockPoolBlock(int aPoolIndex,void** aBlockPointer,BlockHandle* aBlockHandle);

// Deallocates a block from a block pool. It is passed a handle to the
// block, which contains the pool index of the block pool to deallocate the 
// block from. This is used for intrusive blocks.
void deallocateBlockPoolBlock(BlockHandle aBlockHandle);

// Deallocates a block from a block pool. It is passed a pointer to the
// block. It uses the pointer to lookup the block handle, which contains 
// the pool index of the block pool to deallocate the block from. This is
// used for nonintrusive blocks.
void deallocateBlockPoolBlock(void* aBlockPtr);

// Get a pointer to a block, given a block handle.
void* getBlockPoolBlockPtr(BlockHandle aBlockHandle);

// Get the handle of a block, given its address.
BlockHandle getBlockPoolBlockHandle(void* aBlockPtr);

// Shows block pool info.
void showBlockPool(int aPoolIndex);

// Get the size of a block pool.
int getBlockPoolSize(int aPoolIndex);

// Test block pool code.
void testBlockPool(int aPoolIndex);

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Implementation. Constructors.

inline BlockHandle::BlockHandle()
{
   mPoolIndex  = cInvalid;
   mBlockIndex = cInvalid;
}

inline BlockHandle::BlockHandle(unsigned short aPoolIndex, unsigned short aBlockIndex)
{
   mPoolIndex  = aPoolIndex;
   mBlockIndex = aBlockIndex;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Implementation. Set null.

inline void BlockHandle::setNull()
{
   mPoolIndex  = cInvalid;
   mBlockIndex = cInvalid;
}

inline void BlockHandle::set(unsigned short aPoolIndex, unsigned short aBlockIndex)
{
   mPoolIndex  = aPoolIndex;
   mBlockIndex = aBlockIndex;
}

inline bool BlockHandle::isNull()
{
   return mPoolIndex==cInvalid || mBlockIndex==cInvalid;
}

inline BlockHandle  BlockHandle::null()
{
   BlockHandle tTemp;
   tTemp.setNull();
   return tTemp;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Implementation. 
// Set a block handle from the given address of the block. This obtains the 
// block handle by looking it up in the global block pool facility.

inline void BlockHandle::set(void* aBlockPointer)
{
   *this = getBlockPoolBlockHandle(aBlockPointer);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Implementation. 
// Get the address of the block specified by the block handle. This obtains 
// the handle by looking it up in the global block pool facility.

inline void* BlockHandle::ptr(BlockHandle aBlockHandle)
{
   return getBlockPoolBlockPtr(aBlockHandle);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
