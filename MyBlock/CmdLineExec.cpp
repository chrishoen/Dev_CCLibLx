#include "stdafx.h"

#include "ccBlockPool.h"
#include "someBlockPoolIndex.h"
#include "someMyBlockA.h"
#include "someMyBlockB.h"
#include "someMyBlockR.h"

#include "CmdLineExec.h"

using namespace std;

//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
void CmdLineExec::reset()
{
}
//******************************************************************************
void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGoA1(aCmd);

   if(aCmd->isCmd("A1"     ))  executeGoA1(aCmd);
   if(aCmd->isCmd("A2"     ))  executeGoA2(aCmd);
   if(aCmd->isCmd("A3"     ))  executeGoA3(aCmd);
   if(aCmd->isCmd("A4"     ))  executeGoA4(aCmd);

   if(aCmd->isCmd("B1"     ))  executeGoB1(aCmd);
   if(aCmd->isCmd("B2"     ))  executeGoB2(aCmd);

   if(aCmd->isCmd("R1"     ))  executeGoR1(aCmd);
   if(aCmd->isCmd("R2"     ))  executeGoR2(aCmd);

   if(aCmd->isCmd("T1"     ))  executeGoT1(aCmd);
   if(aCmd->isCmd("T2"     ))  executeGoT2(aCmd);
   if(aCmd->isCmd("T3"     ))  executeGoT3(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoA1(Ris::CmdLineCmd* aCmd)
{
   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockA);

   Prn::print(0, "GoA1 %d", sizeof(Some::MyBlockA));
   Some::MyBlockA* tX1 = new Some::MyBlockA(101);
   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockA);
   tX1->method1();

   delete tX1;

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockA);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoA2(Ris::CmdLineCmd* aCmd)
{
   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockA);

   Some::MyBlockA* tX1 = new Some::MyBlockA(101);
   Some::MyBlockA* tX2 = new Some::MyBlockA(102);

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockA);

   delete tX1;
   delete tX2;

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockA);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoA3(Ris::CmdLineCmd* aCmd)
{
   Some::MyBlockA* tX1 = new Some::MyBlockA(101);
   Prn::print(0, "%d %d",tX1->mBlockHandle.mPoolIndex,tX1->mBlockHandle.mBlockIndex);

   CC::BlockHandle tBlockHandle = CC::getBlockPoolBlockHandle(tX1);
   Prn::print(0, "%d %d",tBlockHandle.mPoolIndex,tBlockHandle.mBlockIndex);

   Some::MyBlockA* tX2 = new Some::MyBlockA(102);
   Prn::print(0, "%d %d",tX2->mBlockHandle.mPoolIndex,tX2->mBlockHandle.mBlockIndex);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoA4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoT1(Ris::CmdLineCmd* aCmd)
{
   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockA);

   Some::MyBlockA* tX1 = new Some::MyBlockA(101);
   Prn::print(0, "tX1.mBlockHandle %d %d", tX1->mBlockHandle.mPoolIndex, tX1->mBlockHandle.mBlockIndex);

   Some::MyBlockA* tX2 = (Some::MyBlockA*)CC::getBlockPoolBlockPtr(tX1->mBlockHandle);
   Prn::print(0, "tX2.mBlockHandle %d %d", tX2->mBlockHandle.mPoolIndex, tX2->mBlockHandle.mBlockIndex);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoB1(Ris::CmdLineCmd* aCmd)
{
   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockB);

   Some::MyBlockB* t1B1 = new Some::MyBlockB;

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockB);

   delete t1B1;

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoB2(Ris::CmdLineCmd* aCmd)
{
   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockB);

   Some::MyBlockB* t1B1 = new Some::MyBlockB;
   Some::MyBlockB* t1B2 = new Some::MyBlockB;

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockB);

   delete t1B1;
   delete t1B2;

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoR1(Ris::CmdLineCmd* aCmd)
{
   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockR);

   Some::MyBlockR* tR1 = new Some::MyBlockR;

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockR);

   delete tR1;

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockR);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoR2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoT2(Ris::CmdLineCmd* aCmd)
{
   Some::MyBlockA* tX1 = new Some::MyBlockA(101);
   Some::MyBlockA* tX2 = new Some::MyBlockA(101);
   Some::MyBlockA* tX3 = new Some::MyBlockA(101);
   delete tX1;
   delete tX2;
   delete tX3;

   CC::testBlockPool(Some::cBlockPoolIndex_MyBlockA);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGoT3(Ris::CmdLineCmd* aCmd)
{
   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockA);

   aCmd->setArgDefault(1,1000);
   int tNumBlocks = aCmd->argInt(1);
   for (int i = 0; i < tNumBlocks; i++)
   {
      Some::MyBlockA* tX1 = new Some::MyBlockA(101);
   }

   CC::showBlockPool(Some::cBlockPoolIndex_MyBlockA);
}
