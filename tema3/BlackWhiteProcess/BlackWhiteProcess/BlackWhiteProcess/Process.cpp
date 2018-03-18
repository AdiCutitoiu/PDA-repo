#include "Process.h"
#include "Resource.h"


size_t Process::sProcNumber = 0;

Process::Process(ProcessType aProcessType, Resource & aResource)
: mId{ sProcNumber++ }
, mProcessType{ aProcessType }
, mResource{ aResource }
{
}

size_t Process::GetId() const
{
  return mId;
}

ProcessType Process::GetType() const
{
  return mProcessType;
}

void Process::operator()() const
{
  mResource.Acquire(*this);

  mResource.Use();

  mResource.Release(*this);
}
