/*
  SAudioSource.cc cortizopo@esat-alumni.com
*/

#include "Engine/SAudioSource.h"

#define STB_DEFINE
#include "stb.h"
#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

#include <iostream>

namespace SE
{
  //////////////////////////

  SAudioSource::SAudioSource()
  {
    initialized = false;
    isFading = false;
    isUnfading = false;
  }

  //////////////////////////

  SAudioSource::~SAudioSource()
  {
    if (initialized)
    {
      delete rawData;
      alDeleteSources(1, &source);
      alDeleteBuffers(1, &buffer);
    }
  }

  //////////////////////////

  bool SAudioSource::Init(const char* p, bool doLoop)
  {
    //Now start loading the sound
    unsigned char *mem = stb_fileu((char*)p, &rawMemLenght);

    if (mem == NULL)
    {
      std::cout << "Could not open:" << p << std::endl;
      initialized = false;
      return false;
    }

    //decode a basic memory input into a short raw sound.
    //samples
    //memlen size of the raw data
    //chan number of chanels >1 stereo
    //samplerate 44Hz
    //out output of de decoded mem
    int samples = stb_vorbis_decode_memory(mem, rawMemLenght, &channels, &samplerate, &rawData);
    //std::cout << "Loaded audio\n";

    //Delete loaded data as we already decoded it
    delete mem;

    //Creating a source
    alGenSources(1, &source);

    //Configuring source
    // Position of the source sound.
    ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };

    // Velocity of the source sound.
    curGain = 1.0f;
    ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, curGain);
    alSourcefv(source, AL_POSITION, SourcePos);
    alSourcefv(source, AL_VELOCITY, SourceVel);
    alSourcei(source, AL_LOOPING, doLoop);

    //Buffer
    alGenBuffers(1, &buffer);
    if (channels == 1)
    {
      alBufferData(buffer, AL_FORMAT_MONO8, rawData, sizeof(rawData)*samples, samplerate);
    }
    else if (channels == 2)
    {
      alBufferData(buffer, AL_FORMAT_STEREO16, rawData, sizeof(rawData)*samples, samplerate);
    }

    //Bind 
    alSourcei(source, AL_BUFFER, buffer);

    initialized = true;
    return true;
  }

  //////////////////////////
  float t = 0.0f;
  void SAudioSource::Update(float dt)
  {
    alGetSourcei(source, AL_SOURCE_STATE, &sourceState);

    //Move the audio source
    /*
    float x = sin(t * 0.005f)*15;
    std::cout << x << std::endl;
    ALfloat SourcePos[] = { x, 0.0, 0.0 };
    alSourcefv(source, AL_POSITION, SourcePos);
    */

    //Perform fade/unfade
    if (isFading)
    {
      curGain -= dt;
      alSourcef(source, AL_GAIN, curGain);
      if (curGain <= 0.0f)
      {
        curGain = 0.0f;
        isFading = false;
      }
    }
    if (isUnfading)
    {
      curGain += dt;
      alSourcef(source, AL_GAIN, curGain);
      if (curGain >= 1.0f)
      {
        curGain = 1.0f;
        isUnfading = false;
      }
    }
  }

  //////////////////////////

  void SAudioSource::Play()
  {
    //Play
    alSourcePlay(source);
  }

  //////////////////////////

  void SAudioSource::Pause()
  {
    //Pause
    alSourcePause(source);
  }

  //////////////////////////

  int SAudioSource::GetState()
  {
    //Cache our state
    alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
    return sourceState;
  }

  //////////////////////////

  void SAudioSource::DoFade()
  {
    //Play();
    isFading = true;
  }

  //////////////////////////

  void SAudioSource::DoUnFade()
  {
    //Play();
    isUnfading = true;
  }

  //////////////////////////

  void SAudioSource::SetVolume(float v)
  {
    curGain = v;
    alSourcef(source, AL_GAIN, curGain);
  }

  //////////////////////////

  void SAudioSource::SetLooping(bool l)
  {
    alSourcei(source, AL_LOOPING, l);
  }

  //////////////////////////
}