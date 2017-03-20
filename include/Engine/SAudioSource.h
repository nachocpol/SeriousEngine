/*
  SAudioSource.h cortizopo@esat-alumni.com
*/

#ifndef SAUDIOSOURCE_H
#define SAUDIOSOURCE_H

#include "al.h"

namespace SE
{
  class SAudioSource
  {
  public:
    SAudioSource();
    ~SAudioSource();

    /*
    Initializes this audio source
    @param p The path to the file
    @param doLoop Perform looping?
    @return If succeded
    */
    bool Init(const char* p, bool doLoop);

    /*
    Updates the AudioSource,position and so
    on
    */
    void Update(float dt);

    /*
    Plays this source
    */
    void Play();

    /*
    Pauses this source
    */
    void Pause();

    /*
    Returns the state of this source
    @return Returns the state of this source
    */
    int GetState();

    /*
    Perform volume down
    */
    void DoFade();

    /*
    Perform volume up
    */
    void DoUnFade();

    /*
    Sets the volume of the source
    @param v The value(0-1)
    */
    void SetVolume(float v);

    /*
    Sets if the audio source will loop
    @param l Will loop or not.
    */
    void SetLooping(bool l);


  private:
    //The lenght of the raw data
    size_t rawMemLenght;
    int channels;
    int samplerate;
    short* rawData;
    float curGain;  //0.0f-1.0f

    ALuint source;
    ALuint buffer;

    //Curremt state
    int sourceState;
    bool initialized;

    bool isFading;
    bool isUnfading;
  };
}
#endif