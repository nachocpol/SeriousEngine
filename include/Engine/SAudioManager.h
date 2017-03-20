/*
  SAudioManager.h cortizopo@esat-alumni.com
*/

#ifndef SAUDIOMANAGER_H
#define SADUIOMANAGER_H

#include "al.h"
#include "alc.h"

namespace SE
{
  class SAudioManager
  {
  public:
    SAudioManager();
    ~SAudioManager();

  private:
    ALCcontext* context;
    ALCdevice* device;
  };
}
#endif