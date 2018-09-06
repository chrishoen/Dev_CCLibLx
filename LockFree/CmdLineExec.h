#ifndef _CMDLINEEXEC_H_
#define _CMDLINEEXEC_H_

#include "risCmdLineExec.h"

//******************************************************************************
// specific command line executive
//******************************************************************************

class CmdLineExec : public Ris::BaseCmdLineExec
{
public:

  CmdLineExec();

  void reset();
  void execute(Ris::CmdLineCmd* aCmd);

  void executeGo1(Ris::CmdLineCmd* aCmd);
  void executeGo2(Ris::CmdLineCmd* aCmd);
  void executeGo3(Ris::CmdLineCmd* aCmd);
  void executeRun(Ris::CmdLineCmd* aCmd);

  void executeShow     (Ris::CmdLineCmd* aCmd);
  void executeWriteInt (Ris::CmdLineCmd* aCmd);
  void executeReadInt  (Ris::CmdLineCmd* aCmd);

  int mCount;
};

//******************************************************************************
#endif

