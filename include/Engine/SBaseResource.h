/*
  SBaseResource.h cortizopo@esat-alumni.com
*/

#ifndef SBASERESOURCE
#define SBASERESOURCE

#include <iostream>
#include <string>
#include <memory>

#include "STexture.h"
#include "SMesh.h"

namespace SE
{
  ///////////////////////////////////////////////////////////////////

  class SBaseResource
  {
  public:
    SBaseResource();
    ~SBaseResource();

  protected:
    std::string path;
  };

  ///////////////////////////////////////////////////////////////////

  class STextureResource: public SBaseResource
  {
  public:
    STextureResource();
    ~STextureResource();

    bool Init(std::string p);
    STexture* GetTexture(){ return textureObject; }

  private:
    STexture* textureObject;
  };

  ///////////////////////////////////////////////////////////////////

  class SMeshResource :public SBaseResource
  {
  public:
    SMeshResource();
    ~SMeshResource();

    bool Init(SMeshConstructor mc);
    SMesh* GetMesh(){ return meshObject; }

  private:
    SMesh* meshObject;
  };

  ///////////////////////////////////////////////////////////////////
}

#endif