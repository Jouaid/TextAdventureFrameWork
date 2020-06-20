#include "Game.h"
#include "FactoryFloorState.h"
#include "GameOverState.h"
#include "FactoryState.h"
#include <iostream>
#include "Input.h"
#include "File.h"
#include "json.hpp"
#include "BattleSystem.h"

std::vector<GameState*> Game::gameStates = std::vector<GameState*>();

void Game::CreatePlayer()
{
   player = new Actor();
   player->SetActorStats("PLAYER!", 1, 100, 25, 10); 
}

Game::Game()
{
  //Creates Player
  CreatePlayer();
  // GAMESTATE_FACTORY_FLOOR
  gameStates.push_back(new FactoryFloorState());
  // GAMESTATE_GAMEOVER
  gameStates.push_back(new GameOverState());
  // GAMESTATE_FACTORY
  gameStates.push_back(new FactoryState());
  gameStates.push_back(new BattleSystem());

  stateMachine = new StateMachine<Game>(this);
  stateMachine->ChangeState(Game::GetGameState(GameStateEnum::BATTLE_STATE));
}

Game::~Game()
{
  SAFE_DELETE(stateMachine);
}

ProcessState Game::Update(float deltaTime)
{
  Process::Update(deltaTime);

  if (Input::IsPaused())
    stateMachine->Update();

  //std::cout << "Updated the game";
  return GetState();
}

void Game::Abort()
{
}

void Game::Initialise()
{
  Process::Initialise();

  std::string data;
  File::GetStateDataFromJson("States.json");
}
