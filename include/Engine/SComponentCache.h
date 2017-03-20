/*
  SComponentCache.h cortizopo@esat-alumni.com
*/

#ifndef SCOMPONENTCACHE_H
#define SCOMPONENTCACHE_H

#include "STransform.h"
#include "SMesh.h"
#include "SMaterial.h"
#include "SScript.h"
#include "SLight.h"

#include <vector>

namespace SE
{
  class SGameObject;

  class SComponentCache
  {
  public:

    /*
      Returns the unique instance of the component cache.
      @return Returns the unique instance of the component cache.
    */
    static SComponentCache* GetInstance();

    /*
      Updates the components of the cache.
    */
    void UpdateComponents();

    /*
      Adds a new transform component to the cache.
      @return Returns the id of the transform just created (you should save it)
    */
    void AddTransformComponent(SGameObject* goRef);

    /*
      Adds a mesh to the cache.
      @param goRef A reference to the owner.
    */
    void AddMeshComponent(SGameObject* goRef);

    /*
      Adds a material to the cache.
      @param goRef A reference to the owner.
    */
    void AddMaterialComponent(SGameObject* goRef);

    /*
      Adds a script to the cache.
      @param goRef A reference to the owner.
      @paran p The path of the script.
    */
    void AddScriptComponent(SGameObject* goRef);

    /*
      Adds a light to the cache
      @param goRef A reference to the owner.
    */
    void AddLightComponent(SGameObject* goRef);

    void RemoveTransformComponent(STransform *transformRef);

    void RemoveMeshComponent(SMesh *meshRef);

    void RemoveMaterialComponent(SMaterial *matRef);

    void RemoveScriptComponent(SScript *scriptRef);

    void RemoveLightComponent(SLight *lightRef);

    /*
      Shows the gui of the component cache.
    */
    void ShowGui();

  private:
    SComponentCache();
    ~SComponentCache();
    SComponentCache(SComponentCache const&){}; 
    SComponentCache& operator=(SComponentCache const&){};

    static SComponentCache* instance;

    //Those are the vectors that holds the components
    std::vector<STransform*> transCompCache;
    std::vector<SMesh*> meshCompCache;
    std::vector<SMaterial*> matCompCache;
    std::vector<SScript*> scriptCompCache;
    std::vector<SLight*> lightCompCache;
  };
}
#endif