#ifndef _MPI_PROCESS_H_
#define _MPI_PROCESS_H_

#include "MPIDataType.h"

namespace MPI
{
  class Process
  {
  public:
    /**
     * Constructor
     *
     * @param aCommunicator the communicator of the process
     * @param aRank the rank of the process in the communicator
     */
    Process(const MPI_Comm & aCommunicator, int aRank);

    /**
     * Returns true if this process is the master process
     */
    bool IsMaster() const;

    /**
     * Returns true if this process is the current process
     */
    bool IsCurrentProcess() const;

    /**
     * Returns the rank of the process
     */
    int GetRank() const;

  private:
    const MPI_Comm & mCommunicator;
    const int mRank;
  };
}

#endif // !_MPI_PROCESS_H_

