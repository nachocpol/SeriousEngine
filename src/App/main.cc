/*
  main.cc (SeriousEngineDemo)
*/

#include <iostream>
#include <chrono>
#include <Windows.h>
#include <memory>

//GLM
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Engine/SRenderManager.h"
#include "Engine/SResourceManager.h"
#include "Engine/SScene.h"
#include "Engine/SInput.h"
#include "Engine/SAudioManager.h"
#include "Engine/SAudioSource.h"
#include "Engine/SDebug.h"

bool shouldExit = false;

void ShowPointerData(std::vector<SE::SGameObject> scene);

/////////////////////////

int main()
{
  //Render Manager
  SE::SRenderManager *renderManager;
  renderManager = SE::SRenderManager::GetInstance();
  renderManager->Init(1024, 700, "SeriousEditor");
  renderManager->SetClearColor(glm::vec3(0.6, 0.6, 0.7));
  renderManager->SetClearColor(glm::vec3(0.0f));
  renderManager->SetClearColor(glm::vec3(0.8f,0.8f,1.0f));

  //Resource Manager
  SE::SResourceManager *resourceManager;
  resourceManager = SE::SResourceManager::GetInstance();

  //Camera
  SE::SCamera camera;
  camera.Init(60.0f, 0.01f, 200.0f);
  camera.GetPosition()->y = 3.0f;
  camera.GetPosition()->z = 8.0f;
 
  //Input
  SE::SInput *input = SE::SInput::GetInstance();

  //Scene
  SE::SScene scene;
  scene.SetCamera(camera);
  scene.Init();

  int cInd = -1;

  //Sun
  //Material settings
  SE::SBasicTextureMatSet matSet(
    "../data/shaders/basic_color.vs", "../data/shaders/basic_color.fs", "../data/img/default.png");
  matSet.color = glm::vec3(1.0f, 0.69f, 0.07f);

  cInd = scene.AddGameObject();
  scene.goList[cInd]->Init("Sun");
  scene.goList[cInd]->AddComponent<SE::STransform>();
  scene.goList[cInd]->transform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
  scene.goList[cInd]->transform->SetRotation(glm::vec3(0.0f));
  scene.goList[cInd]->transform->Scale(glm::vec3(1.0f));
  scene.goList[cInd]->AddComponent<SE::SMesh>(SE::SMeshConstructor(SE::kTriangles, "../data/imported/sphere.sm"));
  scene.goList[cInd]->AddComponent<SE::SMaterial>();
  scene.goList[cInd]->material->Init(&matSet);
  scene.goList[cInd]->AddComponent<SE::SLight>();
  scene.goList[cInd]->light->Init(SE::SLightInfo(SE::kSun, glm::vec3(0, -0.5f, -1), glm::vec3(0.0f, 0.0f, 1.0f), 2.0f, 50.0f, 5.0f));
  scene.goList[cInd]->light->SetSceneCenter(glm::vec3(5.0f, 0.0f, 0.0f));

  int SunID = cInd;

  SE::SBasicTextureMatSet matSet2(
    "../data/shaders/color_diffuse.vs", "../data/shaders/color_diffuse.fs", "../data/img/default.png");
  matSet2.refSunPos = scene.goList[SunID]->transform->GetPosition();
  matSet2.refCamPos = camera.GetPosition();
  matSet2.color = glm::vec3(1.0f, 0.0f, 0.0f);

  cInd = scene.AddGameObject();
  scene.goList[cInd]->Init("Red Planet");
  scene.goList[cInd]->AddComponent<SE::STransform>();
  scene.goList[cInd]->transform->SetPosition(glm::vec3(0.0f));
  scene.goList[cInd]->transform->SetRotation(glm::vec3(0.0f));
  scene.goList[cInd]->transform->Scale(glm::vec3(0.5f));
  scene.goList[cInd]->AddComponent<SE::SMesh>(SE::SMeshConstructor(SE::kTriangles, "../data/imported/sphere.sm"));
  scene.goList[cInd]->AddComponent<SE::SMaterial>();
  scene.goList[cInd]->material->Init(&matSet2);

  int RedPlanetID = cInd;

  scene.goList[RedPlanetID]->transform->AttachTo(scene.goList[SunID]);
  scene.goList[RedPlanetID]->transform->Translate(glm::vec3(1.5f, 0.0f, 0.0f));
  scene.goList[RedPlanetID]->transform->SetScale(glm::vec3(0.2f));

  SE::SBasicTextureMatSet matSet3(
    "../data/shaders/color_diffuse.vs", "../data/shaders/color_diffuse.fs", "../data/img/default.png");
  matSet3.refSunPos = scene.goList[SunID]->transform->GetPosition();
  matSet3.refCamPos = camera.GetPosition();
  matSet3.color = glm::vec3(0.0f, 0.0f, 1.0f);

  cInd = scene.AddGameObject();
  scene.goList[cInd]->Init("Blue Planet");
  scene.goList[cInd]->AddComponent<SE::STransform>();
  scene.goList[cInd]->transform->SetPosition(glm::vec3(0.0f));
  scene.goList[cInd]->transform->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));
  scene.goList[cInd]->transform->Scale(glm::vec3(0.5f));
  scene.goList[cInd]->AddComponent<SE::SMesh>(SE::SMeshConstructor(SE::kTriangles, "../data/imported/sphere.sm"));
  scene.goList[cInd]->AddComponent<SE::SMaterial>();
  scene.goList[cInd]->material->Init(&matSet3);

  int BluePlanetID = cInd;

  scene.goList[BluePlanetID]->transform->AttachTo(scene.goList[SunID]);
  scene.goList[BluePlanetID]->transform->Translate(glm::vec3(3.0f, 0.0f, 0.0f));
  scene.goList[BluePlanetID]->transform->SetScale(glm::vec3(0.65f));

  SE::SBasicTextureMatSet matSet4(
    "../data/shaders/color_diffuse.vs", "../data/shaders/color_diffuse.fs", "../data/img/default.png");
  matSet4.refSunPos = scene.goList[SunID]->transform->GetPosition();
  matSet4.refCamPos = camera.GetPosition();
  matSet4.color = glm::vec3(0.0f, 1.0f, 0.0f);

  cInd = scene.AddGameObject();
  scene.goList[cInd]->Init("Green Planet");
  scene.goList[cInd]->AddComponent<SE::STransform>();
  scene.goList[cInd]->transform->SetPosition(glm::vec3(0.0f));
  scene.goList[cInd]->transform->SetRotation(glm::vec3(0.0f));
  scene.goList[cInd]->transform->Scale(glm::vec3(0.5f));
  scene.goList[cInd]->AddComponent<SE::SMesh>(SE::SMeshConstructor(SE::kTriangles, "../data/imported/sphere.sm"));
  scene.goList[cInd]->AddComponent<SE::SMaterial>();
  scene.goList[cInd]->material->Init(&matSet4);

  int GreenPlanetID = cInd;

  scene.goList[GreenPlanetID]->transform->AttachTo(scene.goList[SunID]);
  scene.goList[GreenPlanetID]->transform->Translate(glm::vec3(6.0f, 0.0f, 0.0f));
  scene.goList[GreenPlanetID]->transform->SetScale(glm::vec3(0.85f));

  SE::SBasicTextureMatSet matSet5(
    "../data/shaders/basic_color.vs", "../data/shaders/basic_color.fs", "../data/img/default.png");
  matSet5.color = glm::vec3(1.0f, 1.0f, 1.0f);

  cInd = scene.AddGameObject();
  scene.goList[cInd]->Init("Green Planet Satelite");
  scene.goList[cInd]->AddComponent<SE::STransform>();
  scene.goList[cInd]->transform->SetPosition(glm::vec3(0.0f));
  scene.goList[cInd]->transform->SetRotation(glm::vec3(0.0f));
  scene.goList[cInd]->transform->Scale(glm::vec3(0.5f));
  scene.goList[cInd]->AddComponent<SE::SMesh>(SE::SMeshConstructor(SE::kTriangles, "../data/imported/sphere.sm"));
  scene.goList[cInd]->AddComponent<SE::SMaterial>();
  scene.goList[cInd]->material->Init(&matSet5);

  int GreenSateliteID = cInd;

  scene.goList[GreenSateliteID]->transform->AttachTo(scene.goList[GreenPlanetID]);
  scene.goList[GreenSateliteID]->transform->Translate(glm::vec3(2.0f, 0.0f, 0.0f));
  scene.goList[GreenSateliteID]->transform->SetScale(glm::vec3(0.5f));

  bool scene1 = true;
  while (!shouldExit)
  {
    //Get Window and Input Events
    renderManager->Events();
    renderManager->MainRender();

    if (input->GetKey(SE::kKeySpace))
    {
      if (scene1)
      {
        scene1 = false;
        scene.MoveCamera(glm::vec3(1000.0f, 3.0f, 8.0f));
      }
      else
      {
        scene1 = true;
        scene.MoveCamera(glm::vec3(0.0f, 3.0f, 8.0f));
      }
    }

    if (input->GetKey(SE::kKeyR))
    {
      scene.ReloadShaders();
      //std::cout << ImGui::GetIO().Framerate << std::endl; 
    }
    if (input->GetKey(SE::kKeyEscape))
    {
      shouldExit = true;
    }    

  /*
	sunPosition.x = A * glm::sin(renderManager->GetTime() / 1.0f) + C;
	sunPosition.z = -A * glm::cos(renderManager->GetTime() / 1.0f) + C;
  scene.goList[sunId]->transform->SetPosition(sunPosition);

  sunPosition2.x = A2 * glm::sin((renderManager->GetTime() + 400.0f) / 1.0f) + C2;
  sunPosition2.z = -A2 * glm::cos((renderManager->GetTime() + 500.0f) / 1.0f) + C2;
  scene.goList[sunId2]->transform->SetPosition(sunPosition2);

  sunPosition3.x = A3 * glm::sin((renderManager->GetTime() + 800.0f) / 1.0f) + C3;
  sunPosition3.z = -A3 * glm::cos((renderManager->GetTime() + 800.0f) / 1.0f) + C3;
  scene.goList[sunId3]->transform->SetPosition(sunPosition3);
  */

    //scene.goList[SunID]->transform->Rotate(glm::vec3(0.0f, renderManager->GetDeltaTimeMs() / 50.0f, 0.0f));
    scene.goList[GreenPlanetID]->transform->Rotate(glm::vec3(0.0f, renderManager->GetDeltaTimeMs() / 100.0f, 0.0f));

    scene.Update();
    //if (scene1)
    //scene.RenderToTexture(&cubeMatSet, glm::vec3(1000.0f, 3.0f, 8.0f));
    //else
      scene.Render();

    //Swap buffers
    renderManager->Swap();
  }

  renderManager->ShutDown();
  return 1;
}

/////////////////////////

void ShowPointerData(std::vector<SE::SGameObject> scene)
{
  std::vector<SE::SGameObject> copiedSce(scene);
  for (unsigned int i = 0; i < scene.size(); i++)
  {
    printf("-----Original------\n");
    printf("Pointer of:%01d is:%p.Its material:%p.Its mesh:%p \n",
      i,
      &scene[i],
      scene[i].material,
      scene[i].mesh);
    printf("-----Copied------\n");
    printf("Pointer of:%01d is:%p.Its material:%p.Its mesh:%p \n\n",
      i,
      &copiedSce[i],
      copiedSce[i].material,
      copiedSce[i].mesh);
  }
}

/////////////////////////
