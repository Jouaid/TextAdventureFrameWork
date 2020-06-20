#pragma once
#include <thread>
#include "Process.h"
#include <stack>
#include <vector>
#include "ThreadProcess.h"

typedef std::shared_ptr<Process> StrongProcessPtr;
typedef std::weak_ptr<Process> WeakProcessPtr;
typedef std::vector<StrongProcessPtr> ProcessList;

class ProcessManager
{
private:
  int currentThread;
  ProcessList processList;
  int amountOfThreads;
  std::vector<ThreadProcess> processes;

public:
  ProcessManager(size_t amountOfThreads);
  void StartThread(StrongProcessPtr process);
  //void JoinThread();
  void JoinAll();

  WeakProcessPtr AttachProcess(StrongProcessPtr ptr);
  void UpdateAllProcesses();
};

