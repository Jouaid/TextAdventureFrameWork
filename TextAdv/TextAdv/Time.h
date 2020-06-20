#ifndef __TIME_H__
#define __TIME_H__
#include <chrono>
#include <iostream>
#include "Macros.h"

typedef std::chrono::high_resolution_clock::time_point TimePoint;
//typedef std::chrono::high_resolution_clock clock;

class Time
{
  // Time variable (since startup of program)
  inline static TimePoint time = std::chrono::high_resolution_clock::now();
  inline static TimePoint previousTimeStamp = std::chrono::high_resolution_clock::now();
  inline static float deltaTime = 0;

  // deltaTime calculates render time since last frame

public:
  // Constructor to set start time of the program
  Time()
  {
    time = std::chrono::high_resolution_clock::now();
  }

  static void RecalculateDeltaTime()
  {
    TimePoint now = std::chrono::high_resolution_clock::now();
    auto delta = now - previousTimeStamp;

    deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();

    //SAFE_DELETE(previousTimeStamp);
    previousTimeStamp = now;
  }

  // Get Delta Time (Multi-threaded)
  static float GetDeltaTime()
  {
    return deltaTime;
  }

  // Get Time in seconds since start of game(Multi-threaded)
  static TimePoint GetTime()
  {
    TimePoint newTimeStamp = std::chrono::high_resolution_clock::now();

    return newTimeStamp;
  }

  static void GetFormattedTimeStamp(TimePoint* point = nullptr)
  {
    if (point == nullptr)
      point = &GetTime();

    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now - time;

    std::chrono::hours timeInHours =
      std::chrono::duration_cast<std::chrono::hours>(duration);
    std::chrono::minutes timeInMinutes =
      std::chrono::duration_cast<std::chrono::minutes>(duration);
    std::chrono::seconds timeInSeconds =
      std::chrono::duration_cast<std::chrono::seconds>(duration);


    // DON'T STD::COUT IT
    printf("[%02d:%02d:%02d] ", (int)timeInHours.count(), (int)timeInMinutes.count() % 60, (int)timeInSeconds.count() % 60);
  }
};

#endif // !__TIME_H__