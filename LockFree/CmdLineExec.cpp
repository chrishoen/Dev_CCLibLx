#include "stdafx.h"

#include "prnPrint.h"
#include "LFIndex.h"
#include "LFIntQueue.h"

#include "CmdLineExec.h"

using namespace std;

//******************************************************************************

CmdLineExec::CmdLineExec()
{
   mCount=0;
   LFIntQueue::initialize(4);
}

//******************************************************************************

void CmdLineExec::reset()
{
   for (int i=0;i<100;i++) printf("\n");
   mCount=0;
   LFIntQueue::initialize(4);
}

//******************************************************************************
void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RES"    ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1        (aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2        (aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3        (aCmd);
   if(aCmd->isCmd("RUN"    ))  executeRun        (aCmd);
   if(aCmd->isCmd("SHOW"   ))  executeShow       (aCmd);
   if(aCmd->isCmd("W"      ))  executeWriteInt   (aCmd);
   if(aCmd->isCmd("R"      ))  executeReadInt    (aCmd);
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"%d",sizeof(AtomicLFIndexBlock));;
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeRun(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeShow(Ris::CmdLineCmd* aCmd)
{
   LFIntQueue::show();
}

//******************************************************************************

void CmdLineExec::executeWriteInt(Ris::CmdLineCmd* aCmd)
{
   if (LFIntQueue::tryWrite(++mCount))
   {
      Prn::print(0, "WRITE PASS  $$ %d", mCount);
   }
   else
   {
      Prn::print(0, "WRITE FAIL");
   }
}

//******************************************************************************

void CmdLineExec::executeReadInt(Ris::CmdLineCmd* aCmd)
{
   int tCount=0;
   if (LFIntQueue::tryRead(&tCount))
   {
      Prn::print(0, "READ            PASS  $$ %d", tCount);
   }
   else
   {
      Prn::print(0, "READ            FAIL");
   }
}

