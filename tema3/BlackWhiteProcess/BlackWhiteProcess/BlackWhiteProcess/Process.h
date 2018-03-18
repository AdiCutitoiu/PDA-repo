#ifndef _PROCESS_
#define _PROCESS_

class Resource;

enum class ProcessType
{
  White,
  Black
};

class Process
{
public:
  Process(ProcessType aProcessType, Resource & aResource);

  size_t GetId() const;

  ProcessType GetType() const;

  void operator()() const;

private:
  const size_t mId;
  const ProcessType mProcessType;
  Resource & mResource;

  static size_t sProcNumber;
};

#endif // !_PROCESS_
