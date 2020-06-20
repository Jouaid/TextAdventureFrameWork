#include "GameOverState.h"
#include "Input.h"
#include "FactoryFloorState.h"

void GameOverState::Enter(Game* game)
{
  Input::PrintSentence("You GAY!\r\n Press Enter to RESTART!");
}

void GameOverState::Execute(Game* game, float deltaTime)
{
  std::string input = Input::GetLastSentence();

  if (Input::IsEqual("Play", input))
    game->GetStateMachine()->ChangeState(Game::GetGameState(GameStateEnum::FACTORY_FLOOR));
  else if (Input::IsEqual("Quit", input))
    game->Quit();
}

void GameOverState::Exit(Game* game)
{
}
