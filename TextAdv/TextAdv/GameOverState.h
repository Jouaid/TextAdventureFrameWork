#pragma once

#include "State.h"
#include "Game.h"
#include "GameState.h"

class GameOverState :public GameState
{
public:
  virtual void Enter(Game* game) override;
  virtual void Execute(Game* game, float deltaTime = 0) override;
  virtual void Exit(Game* game) override;
};


