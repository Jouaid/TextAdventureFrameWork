#pragma once
#include "State.h"
#include "Macros.h"

template <class T>
class StateMachine
{
  State<T>* currentState = nullptr;
  State<T>* previousState = nullptr;
  T* owner;

public:
  StateMachine(T* owner) : owner(owner) {}

  void ChangeState(State<T>* newState)
  {
    if (newState == nullptr)
      return;

    if (currentState != nullptr)
    {
      currentState->Exit(owner);
      previousState = nullptr;
      //SAFE_DELETE(previousState); // DO NOT SAFE DELETE IF YOU ARE REUSING THE STATES
      previousState = currentState;
      //SAFE_DELETE(currentState); // DO NOT SAFE DELETE IF YOU ARE REUSING THE STATES
    }

    currentState = newState;
    currentState->Enter(owner);
  }

  void Update(float deltaTime = 0)
  {
    if (currentState != nullptr)
      currentState->Execute(owner);
  }

  void RevertToPreviousState()
  {
    ChangeState(previousState);
  }

  State<T> GetState() { return currentState; }
};


