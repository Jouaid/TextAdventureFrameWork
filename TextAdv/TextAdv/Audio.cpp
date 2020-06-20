#include "Audio.h"
#include "Input.h"
#include "Macros.h"
#include <FMOD\fmod_errors.h>
#include <FMOD\fmod_output.h>
#include <FMOD\fmod.h>
#include <iostream>

// Create an empty pointer to the system
FMOD::System* Audio::system = nullptr;

// Initialise the sound map
Audio::SoundMap Audio::sounds = InitSoundMap();
PlayMessage Audio::pending[MAX_PENDING];

int Audio::numPending = 0;
int Audio::firstAudioChannel = 11;
int Audio::channels = 0;

void Audio::Initialise()
{
  FMOD_RESULT result;
  result = FMOD::System_Create(&system);

  if (result != FMOD_OK)
  {
    Input::PrintSentence("FMOD failed to create it's audio system");
    SetState(STATE_FAILED);
  }
  else if (result == FMOD_OK)
  {
    int numDrivers = 0;
    system->getNumDrivers(&numDrivers);
    for (size_t i = 0; i < numDrivers; i++)
    {
      int driver = 0;
      char name[255];
      FMOD_GUID guid;
      FMOD_SPEAKERMODE mode = FMOD_SPEAKERMODE_STEREO;
      int systemRate = 0;
      int speakerChannels = 0;

      result = system->getDriverInfo(
        system->getDriver(&driver), name, 255, &guid, &systemRate, &mode, &speakerChannels);

      if (result == FMOD_OK)
      {
        std::cout << "Name of driver: " << name << std::endl;
        result = system->init(64, FMOD_INIT_NORMAL, { &driver });
        break;
      }
      else
        continue;
    }

    if (result != FMOD_OK)
    {
      Input::PrintSentence("FMOD failed to initialise");
      SetState(STATE_FAILED);
    }
    else if (result == FMOD_OK)
    {
      Input::PrintSentence("FMOD Setup properly");
      Process::Initialise();

      // Add sounds
      AddSound("Resources\\Sounds\\Dump.wav", SoundType::SOUND_DUMP);
      AddSound("Resources\\Sounds\\LoversTheme.mp3", SoundType::QUALITY_MUSIC);

      system->getSoftwareChannels(&channels);

      std::cout << "Channels: " << channels << std::endl;
    }
    else
    {
      Input::PrintSentence("FMOD issue, aborting process");
      SetState(STATE_ABORT);
    }
  }
}

void Audio::PlaySound(SoundType sound, float volume)
{
  if (!system && numPending >= MAX_PENDING)
    return;

  pending[numPending].id = sound;
  pending[numPending].volume = volume;
  numPending++;
}

void Audio::AddSound(const char* location, SoundType type)
{
  FMOD::Sound* sound = nullptr;

  // Create the sounds
  FMOD_RESULT result = system->createSound(location, FMOD_DEFAULT, NULL, &sound);

  if (result == FMOD_OK)
  {
    // Insert Sound, was loaded successfully
    sounds.insert({ type, sound });
  }
  else
  {
    Input::PrintSentence(std::string("Failed to load sound: ") + std::string(location));
    return;
  }
}

ProcessState Audio::Update(float deltaTime)
{
  ProcessState state = Process::Update();

  system->update();

  if (numPending == 0)
    return STATE_RUNNING;

  std::cout << "Number pending sounds: " << numPending << std::endl;

  int emptyChannel = firstAudioChannel;

  for (size_t i = 0; i < numPending; i++)
  {
    FMOD::Sound* toPlay = sounds[pending[i].id];
    FMOD::Channel* channel = nullptr;

    bool isPlaying = false;
    system->getChannel(emptyChannel, &channel);
    channel->isPlaying(&isPlaying);

    while (isPlaying)
    {
      emptyChannel++;
      system->getChannel(emptyChannel, &channel);
      channel->isPlaying(&isPlaying);
    }

    std::cout << "Channel " << emptyChannel << " to play sound " << (int)pending[i].id << std::endl;

    // Add the volume to the channel, must be between 0 and 1
    float volume = (float)pending[i].volume;
    /*volume = volume > 1 ? 1 : volume;*/

    //channel->setPaused(true);

    FMOD_RESULT result = system->playSound(toPlay, nullptr, false, &channel);

    channel->setVolume(volume);
    channel->setPaused(false);

    if (result == FMOD_OK)
    {
      channel->isPlaying(&isPlaying);

      std::cout << (isPlaying ? "Channel is playing a sound" :
                    "Channel is not playing a sound") <<
        " with volume: " << volume << std::endl;
    }
    else
    {
      std::cout << result;
      throw std::runtime_error("Error while playing sound");
      return STATE_FAILED;
    }
  }

  numPending = 0;

  return state;
}

void Audio::Abort()
{
  SoundMap::iterator it = sounds.begin();
  SoundMap::iterator end = sounds.end();

  while (it != end)
  {
    // Delete the pointers
    ReleaseSound(it->first);
    it++;
  }

  sounds.clear();

  SAFE_DELETE(system);
}

void Audio::ReleaseSound(SoundType sound)
{
  sounds[sound]->release();
  SAFE_DELETE(sounds[sound]);
}
