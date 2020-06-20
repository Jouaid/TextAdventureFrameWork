#ifndef _THREADPROCESS_H_
#define _THREADPROCESS_H_
#include <thread>
#include "Process.h"

class ThreadProcess
{
  std::thread thread;

public:
  ThreadProcess() noexcept { isRunning = false; }
  ThreadProcess(const ThreadProcess&) noexcept { isRunning = false; }         // Copy constructor
  ThreadProcess(ThreadProcess&&) noexcept { isRunning = false; }           // Move constructor
  ThreadProcess& operator=(const ThreadProcess&) noexcept { isRunning = false; }   // Copy assignment operator
  //ThreadProcess& operator=(ThreadProcess&&) {  }      // Move assignment operator
  virtual ~ThreadProcess() {}                        // Destructor
  bool isRunning;

  void Run(Process* process, float deltaSeconds);

  void Stop();
};

#endif