// TextAdv.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include "Game.h"
#include "Macros.h"
#include "Render.h"
#include <stdexcept>
#include <cstdlib>
#include "Process.h"
#include <vector>
#include <memory>
#include <thread>
#include "ProcessManager.h"
#include "Input.h"
#include "ThreadProcess.h"
#include "Time.h"
#include "File.h"
#include "Audio.h"

int main() {
  Time* time = new Time();
  
  StrongProcessPtr file = StrongProcessPtr(new File());
  StrongProcessPtr renderer = StrongProcessPtr(new Render());
  StrongProcessPtr game = StrongProcessPtr(new Game());
  StrongProcessPtr input = StrongProcessPtr(new Input());
  StrongProcessPtr audio = StrongProcessPtr(new Audio());

  StrongProcessPtr processes[] =
  {
    file,
    audio,
    game,
    input,
  };

  // Must be called before anything is working
  audio->Initialise();

  //Get the process count of the processes (to start threads with)
  size_t processCount = sizeof(processes) / sizeof(StrongProcessPtr);

  //ProcessManager keeps track of the current state of processes
  ProcessManager processManager(processCount);

  for (size_t i = 0; i < processCount; i++)
   WeakProcessPtr ptr = processManager.AttachProcess(processes[i]);

  //Does our game break? if so give me an error
  try
  {
    while (!dynamic_cast<Game&>(*game).GetIsQuitting())
    {
      //Update renderer
      renderer->Update();
      Time::RecalculateDeltaTime();

      //Update all other processes
      processManager.UpdateAllProcesses();
    }
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
