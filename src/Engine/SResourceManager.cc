/*
  SResourceManager.cc cortizopo@esat-alumni.com
*/

#include "Engine/SResourceManager.h"
#include "Engine/SDebug.h"

#include <fstream>

namespace SE
{
  //Make sure we only have 1 instance of the class
  SResourceManager* SResourceManager::instance = NULL;

  ///////////////////////////////////////////////////////////////////

  SResourceManager* SResourceManager::GetInstance()
  {
    if (!instance)
    {
      instance = new SResourceManager;
    }
    return instance;
  }

  ///////////////////////////////////////////////////////////////////

  SResourceManager::SResourceManager()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SResourceManager::~SResourceManager()
  {

  }

  ///////////////////////////////////////////////////////////////////

  STexture* SResourceManager::GetTexture(std::string path)
  {
    return textureResourceMap[path]->GetTexture();
  }

  ///////////////////////////////////////////////////////////////////

  bool SResourceManager::AddTexture(std::string path)
  {
    //First check if the texture already exists
    if (textureResourceMap.count(path) == 1)
    {
      SDebug::GetInstance()->Log("%s ,already exist in the resource map.\n ", path.c_str());
      return false;
    }

    //We make a new unique_ptr
    std::unique_ptr<STextureResource> t(new STextureResource);
    //Check if the initialization is valid
    if (!t->Init(path))
    {
      SDebug::GetInstance()->LogError("SResourceManager::AddTexture():Could not add: %s", path.c_str());
      return false;
    }

    //Inset it into the resource map
    textureResourceMap.insert(std::pair<std::string, std::unique_ptr<STextureResource>>(path, std::move(t)));
    SDebug::GetInstance()->Log("Succesfully added(%s) to the resource map.\n", path.c_str());
    return true;
  }

  ///////////////////////////////////////////////////////////////////

  SMesh* SResourceManager::GetMesh(std::string path)
  {
    return meshResourceMap[path]->GetMesh();
  }

  ///////////////////////////////////////////////////////////////////

  bool SResourceManager::AddMesh(SMeshConstructor mc)
  {
    std::string curPath = mc.path;

    //First check if the mesh already exists
    if (meshResourceMap.count(curPath) == 1)
    {
      SDebug::GetInstance()->Log("%s ,already exist in the resource map.", curPath.c_str());
      return true;
    }

    std::ifstream readFile;
    readFile.open(mc.path, std::ios::in | std::ios::binary);
    if (!readFile) 
    { 
      SDebug::GetInstance()->LogError("SResourceManager::AddMesh(): The following file is not found: %s", curPath.c_str());
      return false; 
    }

    //We make a new unique_ptr
    std::unique_ptr<SMeshResource> m(new SMeshResource);
    //Check if the initialization is valid
    if (!m->Init(mc))
    {
      SDebug::GetInstance()->LogError("SResourceManager::AddMesh():Could not add: %s", curPath.c_str());
      return false;
    }

    //Inset it into the resource map
    meshResourceMap.insert(std::pair<std::string, std::unique_ptr<SMeshResource>>(curPath, std::move(m)));
    SDebug::GetInstance()->Log("Succesfully added(%s) to the resource map.", curPath.c_str());

    return true;
  }

  ///////////////////////////////////////////////////////////////////

  void SResourceManager::ClearResources()
  {
    textureResourceMap.clear();
    meshResourceMap.clear();
  }

  ///////////////////////////////////////////////////////////////////
}
