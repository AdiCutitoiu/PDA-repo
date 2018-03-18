#include "Resource.h"
#include "Process.h"
#include <Windows.h>

void Resource::Acquire(const Process & aProcess)
{
  std::unique_lock<std::mutex> lock(mMutex);

  while (!CanBeUsedBy(aProcess))
  {
    mUsersWaiting++;

    mCondVar.wait(lock);

    mUsersWaiting--;
  }

  mUserType = aProcess.GetType();
  ++mUsers;
}

void Resource::Use() const
{
  ::Sleep(500);
}

void Resource::Release(const Process & aProcess)
{
  std::unique_lock<std::mutex> lock(mMutex);

  --mUsers;

  if (mUsers == 0)
    mCondVar.notify_all();
}

bool Resource::CanBeUsedBy(const Process & aProcess)
{
  return mUserType == aProcess.GetType() || mUsers == 0;
}
