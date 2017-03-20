/*
  SComponentCache.cc cortizopo@esat-alumni.com
*/

#include "Engine/SComponentCache.h"
#include "Engine/SGameObject.h"

#include "imgui.h"

#include <algorithm>

namespace SE
{
  SComponentCache* SComponentCache::instance = nullptr;

  ///////////////////////////

  SComponentCache::SComponentCache()
  {

  }

  ///////////////////////////

  SComponentCache::~SComponentCache()
  {

  }

  ///////////////////////////

  SComponentCache* SComponentCache::GetInstance()
  {
    if (!instance)
    {
      instance = new SComponentCache;
    }
    return instance;
  }

  ///////////////////////////

  void SComponentCache::UpdateComponents()
  {
    //Update script component
    for (unsigned int i = 0; i < scriptCompCache.size(); i++)
    {
      scriptCompCache[i]->Update();
    }

    //Update transform component
    for (unsigned int i = 0; i < transCompCache.size(); i++)
    {
      transCompCache[i]->Update();
    }
  }

  ///////////////////////////

  void SComponentCache::AddTransformComponent(SGameObject* goRef)
  {
    transCompCache.push_back(new STransform());
    goRef->transform = transCompCache[transCompCache.size() - 1];
  }

  ///////////////////////////

  void SComponentCache::AddMeshComponent(SGameObject* goRef)
  {
    meshCompCache.push_back(new SMesh());
    goRef->mesh = meshCompCache[meshCompCache.size() - 1];
  }

  ///////////////////////////

  void SComponentCache::AddMaterialComponent(SGameObject* goRef)
  {
    matCompCache.push_back(new SMaterial());
    goRef->material = matCompCache[matCompCache.size() - 1];
  }

  ///////////////////////////

  void SComponentCache::AddScriptComponent(SGameObject* goRef)
  {
    scriptCompCache.push_back(new SScript(goRef));
    goRef->script = scriptCompCache[scriptCompCache.size() - 1];
  }

  ///////////////////////////

  void SComponentCache::AddLightComponent(SGameObject* goRef)
  {
    lightCompCache.push_back(new SLight(goRef));
    goRef->light = lightCompCache[lightCompCache.size() - 1];
  }

  ///////////////////////////

  void SComponentCache::RemoveTransformComponent(STransform *transformRef)
  {
    transCompCache.erase(std::remove(transCompCache.begin(), transCompCache.end(), transformRef), transCompCache.end());
  }

  ///////////////////////////

  void SComponentCache::RemoveMeshComponent(SMesh *meshRef)
  {
    meshCompCache.erase(std::remove(meshCompCache.begin(), meshCompCache.end(), meshRef), meshCompCache.end());
  }

  ///////////////////////////

  void SComponentCache::RemoveMaterialComponent(SMaterial *matRef)
  {
    /*
    std::vector<SMaterial*>::iterator it = std::find(matCompCache.begin(), matCompCache.end(), matRef);
    if (it != matCompCache.end())
    {
      matCompCache.erase(it);
    }
    delete matRef;
    */
    matCompCache.erase(std::remove(matCompCache.begin(), matCompCache.end(), matRef), matCompCache.end());
  }

  ///////////////////////////

  void SComponentCache::RemoveScriptComponent(SScript *scriptRef)
  {
    scriptCompCache.erase(std::remove(scriptCompCache.begin(), scriptCompCache.end(), scriptRef), scriptCompCache.end());
  }

  ///////////////////////////

  void SComponentCache::RemoveLightComponent(SLight *lightRef)
  {
    lightCompCache.erase(std::remove(lightCompCache.begin(), lightCompCache.end(), lightRef), lightCompCache.end());
  }

  ///////////////////////////

  void SComponentCache::ShowGui()
  {
    ImGui::Begin("Component debug");
    {
      ImGui::Separator();
      ImGui::Text("Transforms:%i", transCompCache.size());
      ImGui::Text("Meshes:%i", meshCompCache.size());
      ImGui::Text("Materials:%i", matCompCache.size());
      ImGui::Text("Scripts:%i", scriptCompCache.size());
      ImGui::Text("Lights:%i", lightCompCache.size());
      ImGui::Separator();
    }
    ImGui::End();
  }

  ///////////////////////////
}