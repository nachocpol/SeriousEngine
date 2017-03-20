/*
  STexture.h nachocpol@gmail.com
*/

#ifndef STEXTURE_H
#define STEXTURE_H

#include "STypes.h"

#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace SE
{
  class STexture
  {
  public:
    STexture();
    ~STexture();

    /**
      Initializes this texture object,
      @param path The path to the texture
      @return True if sucessful,false if not
    */
    bool Init(const char *path);

    /**
      Returns the id of the texture object.
      @return The id
    */
    int GetId();

  private:
    GLuint id;
  };
}
#endif