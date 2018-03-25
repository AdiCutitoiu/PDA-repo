#ifndef _MPI_SESSION_H_
#define _MPI_SESSION_H_

#include "Communicator.h"

namespace MPI
{
  class Session
  {
  public:
    /**
     * Destructor
     */
    ~Session();

    /**
     * Copy constructor / assignment
     */
    Session(const Session &) = delete;
    Session& operator=(const Session &) = delete;

    /**
     * Move constructor / assignment
     */
    Session(Session &&) = delete;
    Session& operator=(Session &&) = delete;

    /**
     * True if MPI is initialized successfully
     */
    operator bool() const;

    /**
     * Returns the error code of the MPI initialization
     */
    int GetErrorCode() const;

    /**
     * Returns the world communicator
     */
    Communicator GetWorldCommunicator() const;

  private:
    friend class Handler;

    int mErrorCode;

    /**
     * Constructor
     */
    Session(int & aArgc, char ** & aArgv);

    /**
     * Executes a procedure
     */
    void ExecuteProc(function<void(Session&)> aProc);
  };
}

#endif // !_MPI_SESSION_H_
