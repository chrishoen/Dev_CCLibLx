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

  void executeGoA1(Ris::CmdLineCmd* aCmd);
  void executeGoA2(Ris::CmdLineCmd* aCmd);
  void executeGoA3(Ris::CmdLineCmd* aCmd);
  void executeGoA4(Ris::CmdLineCmd* aCmd);

  void executeGoB1(Ris::CmdLineCmd* aCmd);
  void executeGoB2(Ris::CmdLineCmd* aCmd);

  void executeGoR1(Ris::CmdLineCmd* aCmd);
  void executeGoR2(Ris::CmdLineCmd* aCmd);

  void executeGoT1(Ris::CmdLineCmd* aCmd);
  void executeGoT2(Ris::CmdLineCmd* aCmd);
  void executeGoT3(Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
#endif

