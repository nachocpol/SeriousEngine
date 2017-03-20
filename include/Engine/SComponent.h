/*
  SComponent.h nachocpol@gmail.com
*/

#ifndef SCOMPONENT_H
#define SCOMPONENT_H

#include "STypes.h"

namespace SE
{
  class SComponent
  {
  public:
    SComponent ();
    virtual ~SComponent();
    void Update();
  };
}
#endif