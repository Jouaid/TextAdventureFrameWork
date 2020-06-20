#pragma once
#include "GameState.h"
#include "Actor.h"

enum class BattleStates
{
  PLAYER_TURN,
  AI_TURN,
};

class BattleSystem :public GameState
{
public:
  BattleStates states;

  Game* game;
  Actor* ai;
  Actor* player;

  void SetupBattle();
  void PlayerTurn();
  void AiTurn();
  void Victory();

  virtual void Enter(Game* game) override;
  virtual void Execute(Game* game, float deltaTime = 0) override;
  virtual void Exit(Game* game) override;
};

