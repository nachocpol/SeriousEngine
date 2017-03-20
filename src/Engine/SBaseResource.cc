/*
  SBaseResource.cc cortizopo@esat-alumni.com
*/

#include "Engine/SBaseResource.h"

namespace SE
{
  ///////////////////////////////////////////////////////////////////

  SBaseResource::SBaseResource()
  {
   
  }

  ///////////////////////////////////////////////////////////////////

  SBaseResource::~SBaseResource()
  {

  }

  ///////////////////////////////////////////////////////////////////
  
  STextureResource::STextureResource()
  {

  }

  ///////////////////////////////////////////////////////////////////

  STextureResource::~STextureResource()
  {
    //TO-DO:check if the resource manager failed loading the texture then dont delete it
    printf("Deleting texture resource");
    delete textureObject;
  }

  ///////////////////////////////////////////////////////////////////

  bool STextureResource::Init(std::string p)
  {
    if (p.empty())
    {
      std::cout << "STextureResource::Init():Path was empty!\n";
      return false;
    }
    path = p;
    textureObject = new STexture();
    if (textureObject->Init(path.c_str()))
    {
      return true;
    }
    return false;
  }

  ///////////////////////////////////////////////////////////////////

  SMeshResource::SMeshResource()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SMeshResource::~SMeshResource()
  {
    delete meshObject;
  }

  ///////////////////////////////////////////////////////////////////

  bool SMeshResource::Init(SMeshConstructor mc)
  {
    std::string curPath = mc.path;
    if (curPath.empty())
    {
      std::cout << "SMeshResource::Init():Path was empty!\n";
      return false;
    }
    path = curPath;

    meshObject = new SMesh();
    meshObject->Init(mc);
    return true;
  }

  ///////////////////////////////////////////////////////////////////
}