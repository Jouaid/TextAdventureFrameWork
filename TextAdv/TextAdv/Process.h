#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <iostream>

enum ProcessState
{
  STATE_UNINITIALISED,
  STATE_SUCCEEDED,
  STATE_RUNNING,
  STATE_ABORT,
  STATE_ABORTED,
  STATE_FAILED,
};

#pragma warning( push )
#pragma warning( disable : 26812 )
class Process
{
private:
  ProcessState state;

public:
  Process() { state = STATE_UNINITIALISED; }
  virtual void Initialise() { state = STATE_RUNNING; }
  virtual ProcessState Update(float deltaTime = 0)
  {
    if (state == STATE_UNINITIALISED)
      Initialise();

    return state;
  }

  virtual void Abort() { state = STATE_ABORTED; }

  ProcessState GetState() { return state; }
  void SetState(ProcessState state) { this->state = state; }
};
#pragma warning (pop)

#endif