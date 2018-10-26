#include "stdafx.h"

#include "risThreadsProcess.h"

#include "Parms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize.

void main_initialize(int argc,char** argv)
{
   // Enter process.
   Ris::Threads::setProcessTimerResolution(10);
   Ris::Threads::setProcessAffinityMask(0x3E);
   Ris::Threads::enterProcessHigh();

   // Initialize print facility.
   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::ThreadRun1,  true);
   Prn::setFilter(Prn::ThreadRun2,  false);
   Prn::setFilter(Prn::ThreadRun3,  true);
   Prn::setFilter(Prn::ThreadRun4,  false);

   Prn::setFilter(Prn::ProcRun1,    true);
   Prn::setFilter(Prn::ProcRun2,    true);
   Prn::setFilter(Prn::ProcRun3,    false);
   Prn::setFilter(Prn::ProcRun4,    true);

   Prn::setFilter(Prn::ViewRun1,    true, 1);
   Prn::setFilter(Prn::ViewRun2,    true, 1);
   Prn::setFilter(Prn::ViewRun3,    false,1);
   Prn::setFilter(Prn::ViewRun4,    true, 1);

   Prn::setFilter(Prn::QCallInit1, true);
   Prn::setFilter(Prn::QCallInit2, true);
   Prn::setFilter(Prn::QCallRun1,  false);
   Prn::setFilter(Prn::QCallRun2,  false);
   Prn::setFilter(Prn::QCallRun3,  false);
   Prn::setFilter(Prn::QCallRun4,  false);

   // Read parms files.
   gParms.readFile();

   Prn::print(0,"Concurrent*******************************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize.

void main_finalize()
{
   Prn::print(0,"Concurrent*******************************************END");

   // Close print facility.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
