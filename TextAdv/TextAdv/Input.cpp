#include "Input.h"
#include "Macros.h"
#include <iostream>
#include <algorithm> // for copy() and assign() 
#include <iterator> // for back_inserter 
#include <conio.h>
#include <iostream>

bool Input::inputPause = false;
std::vector<char> Input::buffer;
std::vector<char> Input::lastSentence;

void Input::Initialise()
{
  Process::Initialise();

  buffer.clear();
  lastSentence.clear();
}

ProcessState Input::Update(float deltaTime)
{
  ProcessState state = Process::Update(deltaTime);

  if (inputPause || !_kbhit())
    return state;

  char cur = (char)getchar();

  // Has the user pressed enter?
  if (cur == '\n')
  {
    // If true: 
    // Clear last sentence
    lastSentence.clear();

    buffer.push_back('\0');

    // Copy the buffer to the last sentence
    std::copy(buffer.begin(), buffer.end(), std::back_inserter(lastSentence));

    // Clear buffer for new input
    buffer.clear();

    // inputPause is now true
    inputPause = true;
  }
  else
    buffer.push_back(cur);

  return state;
}

void Input::Abort()
{
  Process::Abort();

  buffer.clear();
  lastSentence.clear();
}



void Input::ClearBuffer()
{
  buffer.clear();
}
