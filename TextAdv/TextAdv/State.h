#pragma once
#include <iostream>

template <class T>
class State
{

public:
  virtual void Enter(T* t) { std::cerr << "You have made a mistake somewhere entering an empy state ENT"; };
  virtual void Execute(T* t, float deltaTime = 0) { std::cerr << "You have made a mistake somewhere entering an empy state EXE"; };
  virtual void Exit(T* t) { std::cerr << "You have made a mistake somewhere entering an empy state EXI"; };
};
