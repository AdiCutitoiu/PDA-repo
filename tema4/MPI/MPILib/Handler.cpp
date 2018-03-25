#include "stdafx.h"
#include "MPI.h"
#include "Handler.h"

using namespace MPI;


bool Handler::Start(int aArgc,
                    char ** aArgv,
                    function<void(Session &)> aMasterProc,
                    function<void(Session &)> aSlaveProc)
{
  static bool mCalled = false;
  if (mCalled)
    return false;

  mCalled = true;

  Session instance(aArgc, aArgv);

  if (!instance)
  {
    instance.GetWorldCommunicator().Abort(instance.GetErrorCode());
    MPI_Finalize();
    return false;
  }

  function<void(Session &)> procToExecute = instance.GetWorldCommunicator().GetCurrentProcess().IsMaster()
                                                ? move(aMasterProc)
                                                : move(aSlaveProc);
  instance.ExecuteProc(move(procToExecute));
  MPI_Finalize();
  return true;
}

