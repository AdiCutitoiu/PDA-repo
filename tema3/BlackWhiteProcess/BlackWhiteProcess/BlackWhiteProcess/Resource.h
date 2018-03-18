#ifndef _RESOURCE_
#define _RESOURCE_

#include <mutex>
#include <condition_variable>
#include <thread>

#include "Process.h"

class Resource
{
public:

  void Acquire(const Process & aProcess);

  void Use() const;

  void Release(const Process & aProcess);

private:
  std::mutex mMutex;
  std::condition_variable mCondVar;

  size_t      mUsersWaiting = 0;
  size_t      mUsers = 0;
  ProcessType mUserType = ProcessType::Black;

  bool CanBeUsedBy(const Process & aProcess);
};

#endif // !_RESOURCE_
