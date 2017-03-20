/*
  STextureCubeMap.h cortizopo@esat-alumni.com
*/

#ifndef STEXTURECUBEMAP_H
#define STEXTURECUBEMAP_H

#include "GL/glew.h"

#include <iostream>
#include <vector>

namespace SE
{
  class STextureCubeMap
  {
  public:
    STextureCubeMap();
    ~STextureCubeMap();

    /*
      Initializes this cubemap with the given array of 
      textures.
    */
    void Init(std::vector<std::string> texPaths);

    /*
      Returns the rendering id of this cubemap.
      @return Returns the rendering id of this cubemap.
    */
    unsigned int GetId(){ return id; }

  private:
    GLuint id;

  };
}
#endif