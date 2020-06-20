#pragma once
#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#include "Game.h"
#include "State.h"
#include <vector>
#include "Actor.h"

enum class Action
{
  NONE = 0,
  SWITCH,
};

class GameState;
struct StateInput
{
public:
  std::string input;
  std::string output;
  std::string prerequisites;
  Action action = Action::NONE;
  GameState* switchToState = nullptr;
};

class GameState : public State<Game>
{
protected:
  std::vector<StateInput> inputs;
public:
  void SetInput(std::vector<StateInput> newInputs) { inputs = newInputs; }
 
  const std::vector<StateInput> GetInput() { return inputs; }
};

#endif // !__GAMESTATE_H__
