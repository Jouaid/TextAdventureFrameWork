#include "FactoryFloorState.h"
#include <iostream>
#include "Input.h"
#include <string>
#include "GameOverState.h"
#include "Audio.h"

void FactoryFloorState::Enter(Game* game)
{
  Input::PrintSentence("You've wandered into a waste factory and you're looking for a toilet.\r\nSomebody locked you in a toilet without toilet paper. The toilet is full of shit so you can't take a dump here. What will you do?\r\n");
}

void FactoryFloorState::Execute(Game* game, float deltaTime)
{
  std::string input = Input::GetLastSentence();
  
  if (input.size() > 0)
  {
  Audio::PlaySound(SoundType::SOUND_DUMP, 100);
    bool correctInput = false;

    for (size_t i = 0; i < inputs.size(); i++)
    {
      if (Input::IsEqual(inputs[i].input, input))
      {
        Input::PrintSentence((inputs[i].output));

        switch (inputs[i].action)
        {
          case Action::SWITCH:
            //GameState* state = Game::GetGameState();
            if (inputs[i].switchToState != nullptr)
              game->GetStateMachine()->ChangeState(inputs[i].switchToState);
            break;
        }

        correctInput = true;
        break;
      }
    }

    if (!correctInput)
      //if(Input::IsEqual("fart", input))
      //{
      //  Input::PrintSentence(std::string("Serious Dave just farted and shat out his own intestines.\r\n"));
      //  game->GetStateMachine()->ChangeState(Game::GetGameState(GameStateEnum::GAMEOVER));
      //}
      //else
    {
      Input::PrintSentence("Wrong Input, try again");
      Input::PrintSentence("You've wandered into a waste factory and you're looking for a toilet.\r\nSomebody locked you in a toilet without toilet paper. The toilet is full of shit so you can't take a dump here. What will you do?\r\n");
    }
  }
}

void FactoryFloorState::Exit(Game* game)
{
}