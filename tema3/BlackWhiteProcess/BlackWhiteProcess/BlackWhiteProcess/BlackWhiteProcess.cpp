#include <cstdio>
#include <vector>
#include "Process.h"
#include "Resource.h"

int main()
{
  Resource r;

  std::vector<std::thread> processes;

  processes.emplace_back(Process(ProcessType::Black, r));
  processes.emplace_back(Process(ProcessType::Black, r));
  processes.emplace_back(Process(ProcessType::Black, r));
  processes.emplace_back(Process(ProcessType::Black, r));
  processes.emplace_back(Process(ProcessType::Black, r));
  processes.emplace_back(Process(ProcessType::White, r));
  processes.emplace_back(Process(ProcessType::White, r));
  processes.emplace_back(Process(ProcessType::White, r));
  processes.emplace_back(Process(ProcessType::White, r));
  processes.emplace_back(Process(ProcessType::White, r));

  return 0;
}

