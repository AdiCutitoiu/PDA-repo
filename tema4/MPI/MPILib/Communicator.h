#ifndef _MPI_COMMUNICATOR_H_
#define _MPI_COMMUNICATOR_H_

#include "Process.h"

namespace MPI
{
  class Communicator
  {
  public:
    Communicator(MPI_Comm comm);

    /**
     * Terminates the execution of all the processes inside the communicator
     */
    void Abort(int aErrorCode) const;

    /**
     * Returns true if this process is the master process
     */
    Process GetCurrentProcess() const;

    /**
     * Returns true if this process is the master process
     */
    Process GetMasterProcess() const;

    /**
     * Returns the slave processes
     */
    vector<Process> GetSlaveProcesses() const;

    /**
     * Gets all the processes
     */
    vector<Process> GetAllProcesses() const;

    /**
     * Returns the communicator size
     */
    int GetSize() const;

    /**
     * Sends data to a destination
     */
    template<typename T>
    bool Send(const Process & aDestination, const vector<T> & aData)
    {
      if (GetCurrentProcess().GetRank() == aDestination.GetRank())
        return false;

      return MPI_Send(reinterpret_cast<void *>(const_cast<T*>(&aData.front())), static_cast<int>(aData.size()),
                      Detail::MPIDataType<T>::ID, aDestination.GetRank(), 1, mCommunicator) == MPI_SUCCESS;
    }

    /**
     * Receives data from a source
     */
    template<typename T>
    optional<vector<T>> Receive(const Process & aSource, int aNoElementsToReceive)
    {
      if (GetCurrentProcess().GetRank() == aSource.GetRank())
        return {};

      vector<int> result(aNoElementsToReceive);

      MPI_Status status;
      MPI_Recv(const_cast<T*>(result.data()), 1, Detail::MPIDataType<T>::ID, aSource.GetRank(), 1, mCommunicator, &status);

      return result;
    }

  private:
    MPI_Comm mCommunicator;
  };
}

#endif // !_MPI_COMMUNICATOR_H_
