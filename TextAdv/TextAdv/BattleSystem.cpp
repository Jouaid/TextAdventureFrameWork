#include "BattleSystem.h"
#include "Input.h"
#include "Audio.h"

void BattleSystem::SetupBattle()
{
  //Get player

  //Make AI or use stored AI
  ai = new Actor();
  ai->SetActorStats("Enemy1", 1, 100, 25, 0);

  player = game->GetPlayer();
  //Set state player goes first
  states = BattleStates::PLAYER_TURN;
}

void BattleSystem::PlayerTurn()
{
  //check what player wants to do with input
  Input::PrintSentence("YOUR TURN YOU CAN: ATTACK OR HEAL");
  //do what it wants
  std::string input = Input::GetLastSentence();

  if (input.size() > 0)
  {
    bool correctInput = false;

    for (size_t i = 0; i < inputs.size(); i++)
    {
      if (Input::IsEqual(inputs[i].input, input))
      {
        Input::PrintSentence((inputs[i].output));
        if (input == "Attack")
        {
          ai->TakeDamage(player->GetDmg());

          //Check if AI died
          if (ai->GetHP() <= 0)
            Victory();
        }
        else if (input == "Heal")
          player->Heal(player->GetDmg());

        correctInput = true;
        break;
      }
    }

    if (!correctInput)
    {
      Input::PrintSentence("Wrong Input, try again");
    }

    //if enemy isnt dead do this
    states = BattleStates::AI_TURN;
  }
}

void BattleSystem::AiTurn()
{
  Audio::PlaySound(SoundType::SOUND_DUMP, 100);

  Input::PrintSentence("ENEMIES TURN");
  int chance = rand() % 100;

  if (chance >= 25)
  {
    player->TakeDamage(ai->GetDmg());

    if (player->GetHP() <= 0)
      game->GetStateMachine()->ChangeState(Game::GetGameState(GameStateEnum::GAME_OVER));
  }
  else
    ai->Heal(ai->GetDmg());

  //if player isnt dead do this
  states = BattleStates::PLAYER_TURN;
}

void BattleSystem::Victory()
{
  //show how much xp the player got
  Input::PrintSentence("XP Gained+++++");
  //Add player gained xp to the player check if it reached above 100
  player->XpGained(25);
  //if it did add extra dmg and a lvl and hp and reset xptolevel
  game->GetStateMachine()->ChangeState(Game::GetGameState(GameStateEnum::FACTORY_FLOOR));
}

void BattleSystem::Enter(Game* game)
{
  this->game = game;
  Input::PrintSentence("Setting up Battle");
  SetupBattle();
}

void BattleSystem::Execute(Game* game, float deltaTime)
{
  switch (states)
  {
    case BattleStates::PLAYER_TURN:
      PlayerTurn();
    case BattleStates::AI_TURN:
      AiTurn();
  }
}

void BattleSystem::Exit(Game* game)
{
}
