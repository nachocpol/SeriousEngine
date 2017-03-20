/*
  STextureCubeMap.cc cortizopo@esat-alumni.com
*/

#include "Engine/STextureCubeMap.h"
#include "Engine/SRenderManager.h"
#include "Engine/SDebug.h"

namespace SE
{
  /////////////////////////////////

  STextureCubeMap::STextureCubeMap()
  {

  }

  /////////////////////////////////

  STextureCubeMap::~STextureCubeMap()
  {
    glDeleteTextures(1, &id);
  }

  /////////////////////////////////
  void STextureCubeMap::Init(std::vector<std::string> texPaths)
  {
    //Generate the texture cube map object
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    //Load each texture of the cubemap
    int x, y,n;
    unsigned char *data = nullptr;
    for (unsigned int i = 0; i < texPaths.size(); i++)
    {
      data = SRenderManager::GetInstance()->LoadTextureFromFile(texPaths[i].c_str(), x, y,n);
      //Check if is valid
      if (data == nullptr)
      {
        SDebug::GetInstance()->LogError("Error loading :%s, into the cubemap.\n", texPaths[i].c_str());
        break;
      }
      SDebug::GetInstance()->Log("Loaded:%s, into the cube map.\n", texPaths[i].c_str());
      //Upload the data
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    //Set some params for the cubemap
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    delete data;
  }

  ///////////////////////////////////////////////////////////////////
}