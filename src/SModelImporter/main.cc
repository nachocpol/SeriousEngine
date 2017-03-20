/*
  main.cc cortizopo@esat-alumni 
          parrau@esat-alumni.com
*/

#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Engine/SRenderManager.h"
#include "SModelImporter/SMIScene.h"
#include "Engine/SMaterial.h"

//#define EXPORTGRID

SE::SScene* scene;
SE::SCamera camera;

typedef std::shared_ptr<SE::SGameObject> GameObject;

//What are we loading
std::string fileToLoad;

//Where do we export it
std::string fileToExport;

//Do override uvs
bool overrideUvs = false;

//Used if we want to override uvs
unsigned int curUvCnt = 0;

//Current loaded mesh
SE::SPhysicalMatSet* matSettings;

int numVertex;
int numUV;
int numTangents;

void GenerateGrid();
///////////////////////////////////////////

int main()
{
#ifdef EXPORTGRID
  GenerateGrid();
  return 0;
#endif

  bool shouldExit = false;

  SE::SRenderManager* renderManager;
  renderManager = SE::SRenderManager::GetInstance();
  renderManager->Init(900, 600, "SModelImporter");
  renderManager->SetClearColor(glm::vec3(0.5f, 0.5f, 0.6f));

  //Input
  SE::SInput *input = SE::SInput::GetInstance();
  //Set the window where the input is going to work
  input->Init(renderManager->GetWindow());

  camera.Init(60, 0.1f, 200.0f);
  scene = new SE::SMIScene();
  scene->SetCamera(camera);
  scene->Init();

  int cInd = -1;
  cInd = scene->AddGameObject();
  scene->goList[cInd]->Init("Light");
  scene->goList[cInd]->AddComponent<SE::STransform>();
  scene->goList[cInd]->transform->SetPosition(glm::vec3(-6.0f, 3.0f, -2.0f));
  scene->goList[cInd]->AddComponent<SE::SLight>();
  scene->goList[cInd]->light->Init(SE::SLightInfo(SE::kSun, glm::vec3(0, -0.5f, -1), glm::vec3(1.0f)));

  //Application loop
  while (!shouldExit)
  {
    renderManager->Events();
    renderManager->MainRender();

    if (input->GetKey(SE::kKeyEscape))
    {
      shouldExit = true;
    }

    if (scene->goList.size() == 2)
    {
      scene->goList[1]->transform->Rotate(glm::vec3(0.0f, (SE::SRenderManager::GetInstance()->GetDeltaTimeMs() * 0.05f), 0.0f));
    }

    scene->Update();
    scene->Render();
    renderManager->Swap();
  }
  return 0;
}

///////////////////////////////////////////

void GenerateGrid()
{
  std::vector<float> vertices;
  std::vector <unsigned int> indices;

  //Gen vertex positions of the grid
  glm::vec3 pos;
  for (int i = -1000; i < 1000; i++)
  {
    //-x/+x
    pos = glm::vec3(-900, 0, i * 4);

    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);

    pos = glm::vec3(900, 0, i * 4);

    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);

    //-z/+z
    pos = glm::vec3(i * 4, 0, -900);

    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);

    pos = glm::vec3(i * 4, 0, 900);

    vertices.push_back(pos.x);
    vertices.push_back(pos.y);
    vertices.push_back(pos.z);
  }

  pos = glm::vec3(0, 900, 0);
  vertices.push_back(pos.x);
  vertices.push_back(pos.y);
  vertices.push_back(pos.z);

  pos = glm::vec3(0, -900, 0);
  vertices.push_back(pos.x);
  vertices.push_back(pos.y);
  vertices.push_back(pos.z);

  //Gen indices of the grid
  for (int i = 0; i < 8002; i++)
  {
    indices.push_back(i);
  }
  
  //Generate data struct
  SE::SBinaryModelData data;
  data.indices = indices;
  data.vertices = vertices;
  data.indicesSize = indices.size();
  data.normalSize = 0;
  data.UVSize = 0;
  data.vertexSize = vertices.size();
  
  //Export
  //ExportSModel("../data/imported/grid.sm", data);
}

///////////////////////////////////////////
