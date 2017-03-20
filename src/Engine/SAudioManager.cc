/*
  SAudioManager.cc cortizopo@esat-alumni.com
*/

#include "Engine/SAudioManager.h"
#include <iostream>

namespace SE
{
  /////////////////////////////////////////////

  SAudioManager::SAudioManager()
  {
    //Initialize all the context/device stuff
    device = alcOpenDevice(NULL);
    if (device == nullptr)
    {
      std::cout << "Could not open device!\n";
      return;
    }

    context = alcCreateContext(device, NULL);
    if (context == nullptr)
    {
      std::cout << "Could not create context!\n";
      return;
    }

    alcMakeContextCurrent(context);
    alGetError();
    std::cout << "Initialized auido engine.\n";
  }

  /////////////////////////////////////////////

  SAudioManager::~SAudioManager()
  {
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
  }

  /////////////////////////////////////////////

}