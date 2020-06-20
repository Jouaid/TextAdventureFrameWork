#ifndef _GAME_H_
#define _GAME_H_
#include "StateMachine.h"
#include "Process.h"
#include "State.h"
#include <vector>
#include "Actor.h"

enum class GameStateEnum
{
  FACTORY_FLOOR,
  FACTORY,
  GAME_OVER,
  BATTLE_STATE,
  MAX,
};

class GameState;
class Game : public Process
{
private:
  StateMachine<Game>* stateMachine;
  bool isQuitting = false;

  static std::vector<GameState*> gameStates;

protected:
  Actor* player;

public:
  void CreatePlayer();
  Actor* GetPlayer() { return player; }

  static GameState* GetGameState(GameStateEnum state)
  {
    if (gameStates.empty() || gameStates.size() < (int)GameStateEnum::MAX)
      return nullptr;

    return gameStates[(int)state];
  }

  StateMachine<Game>* GetStateMachine() { return stateMachine; }

  static GameState* GetGameState(std::string& stateName)
  {
    GameState* state = nullptr;

    if (stateName == "GameOverState")
      state = GetGameState(GameStateEnum::GAME_OVER);
    else if (stateName == "FactoryState")
      state = GetGameState(GameStateEnum::FACTORY);
    else if (stateName == "FactoryFloorState")
      state = GetGameState(GameStateEnum::FACTORY_FLOOR);
    else if (stateName == "BattleState")
      state = GetGameState(GameStateEnum::BATTLE_STATE);
    else
      std::cout << "Wrong state detected: " << stateName << std::endl;

    return state;
  }

  bool GetIsQuitting() { return isQuitting; }
  void Quit() { isQuitting = true; }

  Game();
  ~Game();
  ProcessState Update(float deltaTime = 0);
  void Abort();
  void Initialise() override;
};
#endif

