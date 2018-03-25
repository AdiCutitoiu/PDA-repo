#ifndef _MPI_H_
#define _MPI_H_

#include "Session.h"

namespace MPI
{
  class Handler
  {
  public:
    /**
     * Starts the MPI
     *
     * @param aArgc argument count
     * @param aArgv argument vector
     * @param aMasterProc the procedure executed by the master process
     * @param aSlaveProc the procedure executed by the slave process
     */
    static bool Start(int aArgc, 
                      char ** aArgv, 
                      function<void(Session &)> aMasterProc, 
                      function<void(Session &)> aSlaveProc);
  };
}

#endif // !_MPI_HANDLER_H_
