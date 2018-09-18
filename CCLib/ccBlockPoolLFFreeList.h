#pragma once

/*==============================================================================
Free list block pool class
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <atomic>
#include "ccLFIndex.h"
#include "ccMemoryPtr.h"
#include "ccBlockPool.h"
#include "ccBlockHeader.h"
#include "ccBlockPoolBase.h"

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsulates a block pool that is based on a free list paradigm.
//
// It contains a free list of blocks, which is a block array and a stack of
// indices into the array. When a block is allocated, an index is popped off of
// the stack. When a block is deallocated, its index is pushed onto the stack.
//   
// At initialization, all of the indices are pushed onto the stack. The 
// pool starts out with a full stack.
//
// It allocates memory for the block array and initializes the index stack.
// It is passed the number of blocks to allocate and the size of the blocks. 
//
// For aNumBlocks==10 blocks will range 0,1,2,3,4,5,6,7,8,9
// So block indices will range 0,1,2,3,4,5,6,7,8,9
//
// An index stack is used to manage free list access to the blocks
// The stack is initialized for a free list by pushing indices onto it.
// For aAllocate==10 this will push 9,8,7,6,5,4,3,2,1,0 onto the stack
// so that block zero will be the first one popped.
//
// The index stack can be configured to be thread safe that uses a lock free
// atomic cas treiber stack. It can also configured to be a normal stack
// which is not thread safe, but is faster.

class BlockPoolLFFreeList : public BlockPoolBase
{
public:
   typedef BlockPoolBase BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Class.

   // This returns the number of bytes that an instance of this class
   // will need to be allocated for it.
   static int getMemorySize(BlockPoolParms* aParms);

   class MemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // This marks an invalid free list node.
   static const short int  cInvalid = (short int)0x8000;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If this flag is false then the memory for this object was created
   // externally. If it is true then the memory was allocated at 
   // initialization and must be freed at finalization.
   bool mOwnMemoryFlag;

   // Pointer to memory for which the stack resides. This is either created
   // externally and passed as an initialization parameter or it is created
   // on the system heap at initialization.
   void* mMemory;

   // This allocates storage for the blocks on the system heap or in shared
   // memory and provides pointer access to the allocated blocks. This is a block
   // box array. A block box contains a block header and a block body. The
   // header is invisible to the user and is used for things like resource
   // counting and pointer to handle conversions. The block body is visible to 
   // the user as a pointer to the block.
   //
   // Pointer to allocated memory for the block box array.
   // This is an array of bytes of size NumBlocks*BlockBoxSize.
   char* mBlockBoxArray;
   // Same thing.
   MemoryPtr mBlockBoxArrayPtr;

   // Free List array for treiber stack.
   AtomicLFIndex*    mFreeListNext;

   // A pointer to the parameters that were passed in at initialization.
   // Whoever owns this block pool (creates and initializes it) must maintain
   // storage for these parameters for the lifetime of the block pool.
   // The owner creates an instance of the parameters and fills in some of
   // them and passes them to the block pool at initialization. The block pool
   // then also fills in some of them during its initiialization and the 
   // owner might use some of them after the initialization. Memory storage
   // for these must be maintained my the owner throughout the lifetime of
   // the block pool.
   BlockPoolParms* mParms;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Number of blocks allocated.
   int mNumBlocks;

   // Size of each block allocated.
   int mBlockSize;

   // Size of each block box allocated.
   int mBlockBoxSize;

   // Memory pool index for the block box array.
   int mPoolIndex;

   // Number of free list nodes allocated
   int mFreeListNumNodes;

   // Free list head node.
   AtomicLFIndex mFreeListHead;

   // Free list size.
   std::atomic<int> mFreeListSize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BlockPoolLFFreeList();
  ~BlockPoolLFFreeList();

   // Initialize the block pool. It is passed block pool parameters.
   void initialize(BlockPoolParms* aParms);

   // Deallocate memory for the block pool.
   void finalize();

   // Get a block from the pool, this allocates a block.
   // Return true if successful, false if the block pool is empty.
   bool allocate(void** aBlockPointer,BlockHandle* aBlockHandle);

   // Put a block back to the pool, this deallocates a block.
   void deallocate(BlockHandle aBlockHandle);

   // Get a pointer to a block, given its memory handle.
   void* getBlockPtr(BlockHandle aBlockHandle);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Block box methods.

   // Return a pointer to a block box, based on its block index.
   char* getBlockBoxPtr(int aIndex);

   // Return a pointer to a block header, based on its block index.
   BlockHeader* getHeaderPtr(int aIndex);

   // Return a pointer to a block body, based on its block index.
   char* getBlockPtr(int aIndex);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Free list methods.

   // Pop a value off of the stack. Return false if the stack is empty.
   bool listPop(int* aValue);

   // Push a value onto the stack. Return false if the stack is full.
   bool listPush(int aValue);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Helper methods.

   // Size, the number of blocks that are available to be allocated.
   int size();

   // Show status and metrics for the block pool.
   void show();

   // Get the handle of a block, given its address.
   static BlockHandle getBlockHandle(void* aBlockPtr);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

