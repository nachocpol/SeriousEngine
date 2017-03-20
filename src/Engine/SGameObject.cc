/*
  SGameObject.cc nachocpol@gmail.com & David Parra
*/

#include "Engine/SGameObject.h"
#include "Engine/SResourceManager.h"
#include "Engine/SComponentCache.h"
#include "Engine/SLight.h"
#include "Engine/SMaterial.h"
#include "Engine/SMesh.h"
#include "Engine/SScript.h"
#include "Engine/SDebug.h"

namespace SE
{
  /////////////////////////////////////////////////

  SGameObject::SGameObject()
  {
    //By default we create a transform for each game object
    //transform = new STransform;
    compCache = SComponentCache::GetInstance();
  }

  /////////////////////////////////////////////////

  SGameObject::~SGameObject()
  {
    
  }

  /////////////////////////////////////////////////

  void SGameObject::Init(std::string goName)
  {
    myName = goName;
  }

  /////////////////////////////////////////////////
  
  void SGameObject::Update()
  {

  }

  /////////////////////////////////////////////////

  //Implementation for the transform component
  template<> void SGameObject::AddComponent<STransform>()
  {
    if (transform)
    {
      SE::SDebug::GetInstance()->LogWarning("You are trying to add a transform but we already have one!");
      return;
    }
    compCache->AddTransformComponent(this);
  }

  template<> void SGameObject::RemoveComponent<STransform>()
  {
    if (transform)
    {
      compCache->RemoveTransformComponent(transform);
      transform = nullptr;
    }
  }

  //Implementation for the mesh component (it will be procedural mesh)
  template<> void SGameObject::AddComponent<SMesh>()
  {
    if (mesh)
    {
      //std::cout << "You are trying to add a mesh but we already have one!\n";
      return;
    }

    compCache->AddMeshComponent(this);
  }

  //Implementation for the mesh component with mesh constructor
  template<> bool SGameObject::AddComponent<SMesh>(SMeshConstructor mc)
  {
    if (mesh)
    {
      //std::cout << "You are trying to add a mesh but we already have one!\n";
      return true;
    }
    if (SResourceManager::GetInstance()->AddMesh(mc))
    {
      mesh = SResourceManager::GetInstance()->GetMesh(mc.path);
      return true;
    }
    return false;
    //compCache->AddMeshComponent(this);
  }

  /////////////////////////////////////////////////

  template<> void SGameObject::RemoveComponent<SMesh>()
  {
    if (mesh)
    {
      compCache->RemoveMeshComponent(mesh);
      mesh = nullptr;
    }
  }

  /////////////////////////////////////////////////

  //Implementation for the material component
  template<> void SGameObject::AddComponent<SMaterial>()
  {
    if (material)
    {
      SE::SDebug::GetInstance()->LogWarning("You are trying to add a material but we already have one!");
      return;
    }
    compCache->AddMaterialComponent(this);
  }

  /////////////////////////////////////////////////

  template<> void SGameObject::RemoveComponent<SMaterial>()
  {
    if (material)
    {
      compCache->RemoveMaterialComponent(material);
      material = nullptr;
    }
  }

  /////////////////////////////////////////////////

  //Implementation for the light component
  template<> void SGameObject::AddComponent<SLight>()
  {
    if (light)
    {
      SE::SDebug::GetInstance()->LogWarning("You are trying to add a light but we already have one!");
      return;
    }
    compCache->AddLightComponent(this);
  }

  /////////////////////////////////////////////////

  template<> void SGameObject::RemoveComponent<SLight>()
  {
    if (light)
    {
      compCache->RemoveLightComponent(light);
      light = nullptr;
    }
  }

  /////////////////////////////////////////////////

  template<> void SGameObject::AddComponent<SScript>()
  {
    if (script)
    {
      SE::SDebug::GetInstance()->LogWarning("You are trying to add a script but we already have one!");
      return;
    }
    compCache->AddScriptComponent(this);
  }

  /////////////////////////////////////////////////

  template<> void SGameObject::RemoveComponent<SScript>()
  {
    if (script)
    {
      compCache->RemoveScriptComponent(script);
      script = nullptr;
    }
  }

  /////////////////////////////////////////////////

  void SGameObject::DeleteGameObject()
  {
    RemoveComponent<SScript>(); 
    RemoveComponent<SLight>();
    RemoveComponent<SMaterial>();
    RemoveComponent<SMesh>();
    RemoveComponent<STransform>();
  }

  /////////////////////////////////////////////////
}