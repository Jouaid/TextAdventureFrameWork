#pragma once
#include "Process.h"
#include <vector>
#include <string>
#include <atomic>
#include "Time.h"

class Input :
  public Process
{
  static std::vector<char> buffer;
  static std::vector<char> lastSentence;
  static bool inputPause;

public:
  void Initialise() override;
  ProcessState Update(float deltaTime = 0) override;
  void Abort() override;

  static std::string GetLastSentence()
  {
    // copy lastsentence to a string and return it
    std::string sentence(lastSentence.begin(), lastSentence.end());

    // Unpause input
    inputPause = false;

    return sentence;
  }

  static bool IsPaused()
  {
    return inputPause;
  }

  static void PrintSentence(std::string sentence, std::string sentence1)
  {
    Time::GetFormattedTimeStamp();

    std::cout << sentence << sentence1 << std::endl;
  }

  static void PrintSentence(std::string sentence)
  {
    Time::GetFormattedTimeStamp();

    std::cout << sentence << std::endl;
  }

  static void PrintSentence(const char* sentence)
  {
    Time::GetFormattedTimeStamp();

    std::cout << sentence << std::endl;
  }

  static bool IsEqual(const char* left, std::string right)
  {
    return strcmp(left, ToLower(right.c_str()).c_str()) == 0;
  }

  static bool IsEqual(std::string left, std::string right)
  {
    return IsEqual(ToLower(left.c_str()).c_str(), ToLower(right.c_str()));
  }

  static std::string ToLower(std::string text)
  {
    for (size_t i = 0; i < text.size(); i++)
      text[i] = std::tolower(text[i]);

    return text;
  }

  void ClearBuffer();
};


