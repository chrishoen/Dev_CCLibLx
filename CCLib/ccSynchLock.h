#pragma once

/*==============================================================================

Realtime infastructure library Threads

This file contains classes that encapsulate standard rtos multithreading
synchronization constructs. It supplies events, semaphores, and threads.
The purpose of the classes is to wrap the rtos api thread synchronization
calls.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CC
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a synchronization lock that is used to protect access
// to critical sections of code.

class SynchLock
{
public:

   SynchLock();
   ~SynchLock();
      
   void lock();
   void unlock();

protected:
   class Specific;
   Specific* mSpecific;
};
   
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


