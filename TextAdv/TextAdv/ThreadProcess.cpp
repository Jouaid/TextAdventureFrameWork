#include "ThreadProcess.h"

void ThreadProcess::Run(Process* process, float deltaSeconds)
{
  std::thread newThread(&Process::Update, process, deltaSeconds);
  thread.swap(newThread);
  isRunning = true;
}

void ThreadProcess::Stop()
{
  thread.join();
  isRunning = false;
}