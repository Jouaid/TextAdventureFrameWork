#pragma once
#include "Process.h"
#include <FMOD/fmod.h>
#include <FMOD/fmod_common.h>
#include <FMOD/fmod.hpp>
#include <map>

enum class SoundType
{
  SOUND_NONE,
  SOUND_DUMP,
  QUALITY_MUSIC,
};

struct PlayMessage
{
  SoundType id;
  float volume;
};

class Audio :
  public Process
{
private:
  static FMOD::System* system;
  static FMOD_VECTOR listener;
  typedef std::map<SoundType, FMOD::Sound*> SoundMap;
  static SoundMap sounds;

  static SoundMap InitSoundMap() { return SoundMap(); }
  void AddSound(const char* location, SoundType type);

  static const int MAX_PENDING = 16;
  static PlayMessage pending[MAX_PENDING];
  static int numPending;
  static int channels;
  // We decided that channels 1-10 would be used for music
  static int firstAudioChannel;


public:
  static void PlaySound(SoundType sound, float volume);
  virtual void Initialise() override;
  virtual ProcessState Update(float deltaTime = 0) override;
  virtual void Abort() override;
  void ReleaseSound(SoundType sound);
};

