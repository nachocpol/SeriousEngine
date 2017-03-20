/*
  SResourceManager.h cortizopo@esat-alumni.com
*/

#ifndef SRESOURCEMANAGER_H
#define SRESOURCEMANAGER_H

#include <map>
#include <memory>

#include "SBaseResource.h"

namespace SE
{
  ////////////////////////////

  class SResourceManager
  {
  public:
    SResourceManager();
    ~SResourceManager();

    /**
      Returns the single instance of the Resource Manager
      @return returns the single instance of the Resource Manager
    */
    static SResourceManager* GetInstance();

    /**
      Adds a new texture to the resource manager, the path is used as a key to find the loaded texture
      @return false if the texture is duplicated or if it could not be find and true if everithing is correct
    */
    bool AddTexture(std::string path);

    /**
      Returns the texture added before using the path added as a key
      @return The texture reference pointer
    */
    STexture* GetTexture(std::string path);

    /**
      Adds a new mesh to the resource manager, the path is used as a key to find the mesh
      @return false if the mesh is duplicated or if it colud not be find
    */
    bool AddMesh(SMeshConstructor mc);

    /**
      Returns the mesh added before using the path added as a key
      @return The mesh reference pointer
    */
    SMesh* GetMesh(std::string path);

    /*
      Clears ALL the resources in the resource manager YOLO SWAG.
    */
    void ClearResources();

  private:
    SResourceManager(SResourceManager const&){};
    SResourceManager& operator= (SResourceManager const&){};
    static SResourceManager *instance;

    std::map<std::string, std::unique_ptr<STextureResource>> textureResourceMap;
    std::map<std::string, std::unique_ptr<SMeshResource>> meshResourceMap;
  };

  ////////////////////////////
}
#endif
