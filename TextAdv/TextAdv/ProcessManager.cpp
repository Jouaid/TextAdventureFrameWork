#include "ProcessManager.h"
#include "Time.h"

WeakProcessPtr ProcessManager::AttachProcess(StrongProcessPtr ptr)
{
  WeakProcessPtr weak = ptr;

  processList.push_back(ptr);

  return ptr;
}

void ProcessManager::UpdateAllProcesses()
{
  // Have the first iterator start at the beginning
  std::vector<StrongProcessPtr>::iterator it = processList.begin();
  // Have an iterator at the end
  std::vector<StrongProcessPtr>::iterator end = processList.end();

  size_t failCount = 0;
  size_t successCount = 0;

  // Continue to iterate until we reach the end
  while (it != end)
  {
    switch ((*it)->GetState())
    {
      case STATE_ABORT:
        (*it)->Abort();
        break;
      case STATE_UNINITIALISED:
        (*it)->Initialise();
        successCount++;
        break;
      case STATE_RUNNING:
        StartThread(*it);
        successCount++;
        // Add to running processes
        break;
      case STATE_ABORTED:
        // Remove aborted process
        // Add to failed processes
        break;

      case STATE_FAILED:
        failCount++;
        break;
      default:
        break;
    }

    // Select next iteration point so that we can reach the end
    it++;
  }

  //std::cout << "Successfully ran the processes\r\n";

  int data = ((successCount << processList.size()) | failCount);
  JoinAll();
}

ProcessManager::ProcessManager(size_t amountOfThreads)
{
  this->amountOfThreads = amountOfThreads;

  currentThread = 0;

  for (size_t i = 0; i < amountOfThreads; i++)
  {
    ThreadProcess newProcess;
    processes.push_back(newProcess);
  }
}

void ProcessManager::StartThread(StrongProcessPtr process)
{
  // First run the process in a new thread
  float deltaTime = Time::GetDeltaTime();
  processes[currentThread].Run(&(*process), deltaTime);

  // Then add to currentThread;
  currentThread++;
}

void ProcessManager::JoinAll()
{
  size_t lastThread = currentThread;

  for (size_t i = 0; i < lastThread; i++)
    processes[i].Stop();

  // Reset the current thread to the first thread to be used
  currentThread = 0;
}