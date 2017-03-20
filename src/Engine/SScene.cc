/*
  SScene.cc nachocpol@gmail.com && David Parra
*/

#include "Engine/SScene.h"
#include "Engine/STransform.h"
#include "Engine/SResourceManager.h"
#include "Engine/SDebug.h"
#include "Engine/SRenderManager.h"
#include "Engine/SSceneParser.h"

#include "gtc/matrix_transform.hpp"

#include <windows.h>
#include <iostream>
#include <fstream>

namespace SE
{

  ///////////////////////////////////////////////////////////////////

  SScene::SScene()
  {
    //Initialize the display list
    displayList = new SDisplayList();

    //Save a reference to the render manager
    renderManRef = SRenderManager::GetInstance(); 

    //Init the global cube map
    std::vector<std::string> textures;
    textures.push_back("../data/img/cubemap/right.jpg");
    textures.push_back("../data/img/cubemap/left.jpg");
    textures.push_back("../data/img/cubemap/top.jpg");
    textures.push_back("../data/img/cubemap/bottom.jpg");
    textures.push_back("../data/img/cubemap/back.jpg");
    textures.push_back("../data/img/cubemap/front.jpg");
    renderManRef->InitCubeMap(textures);
  }

  ///////////////////////////////////////////////////////////////////

  SScene::~SScene()
  {
    glDeleteBuffers(1, &dDataBuffer);
    delete displayList;
  }

  ///////////////////////////////////////////////////////////////////

  void SScene::Init()
  {
    //Uniform buffer for draw data (0)
    glGenBuffers(1, &dDataBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, dDataBuffer);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, dDataBuffer, 0, 2 * sizeof(glm::mat4));

    displayList->Init();

    //Init shado fbo
    //shadowFBOMatSet = new SMaterialSettings("../data/shaders/BScreen.vs", "../data/shaders/BScreen.fs");
    //shadowFBO = new SFrameBuffer(SScreenQuadConstructor(),shadowFBOMatSet);

    //Init final FBO
    finalFboMs = new SE::SBaseScreenMaterialSet("../data/shaders/BScreen.vs", "../data/shaders/BScreen.fs");
    finalFbo = new SE::SFrameBuffer(SE::SScreenQuadConstructor(/*glm::vec2(-1,-1),glm::vec2(0,-1),glm::vec2(0,1),glm::vec2(-1,1)*/), finalFboMs);
    finalFboMs->screenColorTexture = finalFbo->GetColorTex();

    finalFboMs2 = new SE::SBaseScreenMaterialSet("../data/shaders/depthPostprocess.vs", "../data/shaders/depthPostprocess.fs");
    finalFbo2 = new SE::SFrameBuffer(SE::SScreenQuadConstructor(/*glm::vec2(-1,-1),glm::vec2(0,-1),glm::vec2(0,1),glm::vec2(-1,1)*/), finalFboMs2);
    finalFboMs2->screenColorTexture = finalFbo2->GetColorTex();

    //This is temporal
    defaultMatSet = new SE::SPhysicalMatSet(
      "../data/shaders/seriousPS.vs", "../data/shaders/seriousPS.fs", 
      "../data/img/default.png", "../data/img/default.png", "../data/img/default.png", "../data/img/default.png");

    defaultMatSet->refCamPos = mainCamera->GetPosition();

    cameraGO = AddGameObject("Camera");
    goList[cameraGO]->AddComponent<SE::SLight>();
    goList[cameraGO]->light->Init(SE::SLightInfo(SE::kSun, glm::vec3(0, 0, 0), glm::vec3(0.0f), 1.0f, 120.0f, 60.0f));

  }

  ///////////////////////////////////////////////////////////////////

  void SScene::SetCamera(SCamera &c)
  {
    mainCamera = &c;
  }

  void SScene::MoveCamera(glm::vec3 p)
  {
    mainCamera->SetPosition(p);
  }

  ///////////////////////////////////////////////////////////////////

  int SScene::AddGameObject()
  {
    goList.push_back(new SGameObject());
    return goList.size() - 1;
  }

  ///////////////////////////////////////////////////////////////////

  void SScene::DeleteGameObject(const int id)
  {
    goList[id]->DeleteGameObject();
    goList.erase(goList.begin() + id);
    curSelectedGo = -1;
  }

  ///////////////////////////////////////////////////////////////////

  void SScene::Update()
  {
    //Update the main camera
    if (mainCamera == nullptr)
    {
      std::cout << "SScene::Update():main camera is 'NULL',set it with SetCamera()\n";
      return;
    }
    mainCamera->Update();

    if (SE::SInput::GetInstance()->GetKey(SE::kKeyF12))
    {
      if (hideGUI) { hideGUI = false; }
      else { hideGUI = true; }
    }

    //Update the component cache
    SE::SComponentCache::GetInstance()->UpdateComponents();

    //Update the game objects
    for (unsigned int i = 0; i < goList.size(); i++)
    {
      goList[i]->Update();
    }

    //Start a task to fill the display list
    SE::STaskHandler task;
    task = SE::STaskManager::GetInstance()->AddTask(
    std::unique_ptr<SE::STask_UpdateDL>(new SE::STask_UpdateDL(*displayList, goList)));

    //Save the lights of the scene
    for (unsigned int i = 0; i < goList.size(); i++)
    {
      if (goList[i]->light && i != cameraGO)
      {
        lights.push_back(goList[i]->light);
      }
    }
    
    //Wait to fill the dl
    task.Wait();
  }

  ///////////////////////////////////////////////////////////////////
  //Get Depth of the scene from the camera
  /*
    goList[cameraGO]->transform->SetPosition(*mainCamera->GetPosition());
    goList[cameraGO]->light->SetLightDir(mainCamera->GetDirection());

    displayList->SetShadowPass(true);
    {
      SE::SLightInfo lInfo = goList[cameraGO]->light->GetLightInfo();

      SRenderManager  *renderManager = SRenderManager::GetInstance();
      glm::vec2 ws = renderManager->GetWindowSize();
      float aspect = ws.x / ws.y;

      glm::mat4 projection = glm::perspective(glm::radians(lInfo.lSize), aspect, lInfo.lNear, lInfo.lFar);
            
      displayList->SetCurLightMatrix(projection * *mainCamera->GetView());
      goList[cameraGO]->light->EnableLightShadow();
      displayList->RunDl();
      goList[cameraGO]->light->DisableLightShadow();
    }
    displayList->SetShadowPass(false);

    finalFboMs2->depthTexture = goList[cameraGO]->light->GetShadowTexure();
  */

  ///////////////////////////////////////////////////////////////////

  void SScene::Render()
  {
    //Upload the view and projection to the uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, dDataBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetView()));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetProjection()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Render the shadow maps 
    displayList->SetShadowPass(true);
    for (unsigned int i = 0; i<lights.size(); i++)
    {
      displayList->SetCurLightMatrix(lights[i]->GetLightMatrix());
      lights[i]->EnableLightShadow();
      displayList->RunDl();
      lights[i]->DisableLightShadow();
    }
    displayList->SetShadowPass(false);

    finalFbo->Enable();
    {
      displayList->SetCurrentLights(lights);
      displayList->SetCamera(mainCamera);
      displayList->RunDl();
    }
    finalFbo->Disable();

    finalFbo->Draw();

    RenderGUI();

    displayList->CleanDl();
    lights.clear();
  }

  void SScene::RenderToTexture(SBasicTextureMatSet *mat, glm::vec3 sceneToRender)
  {
    //Upload the view and projection to the uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, dDataBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetView()));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetProjection()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Render the shadow maps 
    displayList->SetShadowPass(true);
    for (unsigned int i = 0; i<lights.size(); i++)
    {
      displayList->SetCurLightMatrix(lights[i]->GetLightMatrix());
      lights[i]->EnableLightShadow();
      displayList->RunDl();
      lights[i]->DisableLightShadow();
    }
    displayList->SetShadowPass(false);

    glm::vec3 lastCameraPos = *mainCamera->GetPosition();
    MoveCamera(sceneToRender);
    glBindBuffer(GL_UNIFORM_BUFFER, dDataBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetView()));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetProjection()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Render the scene into a fbo
    finalFbo->Enable();
    {
      displayList->SetCurrentLights(lights);
      displayList->SetCamera(mainCamera);
      displayList->RunDl();
    }
    finalFbo->Disable();
    mat->lightTexture = finalFbo->GetColorTex();

    MoveCamera(lastCameraPos);
    glBindBuffer(GL_UNIFORM_BUFFER, dDataBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetView()));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetProjection()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    displayList->SetCurrentLights(lights);
    displayList->SetCamera(mainCamera);
    displayList->RunDl();

    //Render the gui
    RenderGUI();

    displayList->CleanDl();
    lights.clear();
  }

  ///////////////////////////////////////////////////////////////////

  void SScene::ReloadShaders()
  {
    for (unsigned int i = 0; i < goList.size(); i++)
    {
      if(goList[i]->material)
      {
        goList[i]->material->Reload();
      }
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SScene::RenderGUI()
  {
    static bool show = true;
    static bool showDebugInfo = true;
    static bool showConsole = true;
    static bool showScriptEditor = false;
    static bool showScriptCreator = false;
    static bool showMeshCreator = false;
    static bool showGameObjectCreator = false;
    static bool showErrorNotGOSelected = false;
    static bool showGONameEditor = false;
    static bool showSceneSaver = false;
    static bool showSceneLoader = false;
    static char scriptText[1024 * 16];
    //ImGui::ShowTestWindow(&show);

    // Menu
    if (!hideGUI)
    {
      if (ImGui::BeginMainMenuBar())
      {
        if (ImGui::BeginMenu("File"))
        {
          if (ImGui::MenuItem("Load Scene", NULL, false)) { showSceneLoader = true; }
          if (ImGui::MenuItem("Save Scene", NULL, false)) { showSceneSaver = true; }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("GameObject"))
        {
          if (ImGui::MenuItem("Add GameObject", NULL, false))
          {
            showGameObjectCreator = true;
          }
          if (ImGui::BeginMenu("Add Component"))
          {
            if (curSelectedGo != -1) {
              if (ImGui::MenuItem("Mesh", NULL, false, goList[curSelectedGo]->mesh == nullptr))
              {
                showMeshCreator = true;
              }
              if (ImGui::MenuItem("Material", NULL, false, true))
              {
                goList[curSelectedGo]->RemoveComponent<SE::SMaterial>();
                goList[curSelectedGo]->AddComponent<SE::SMaterial>();
                goList[curSelectedGo]->material->CreateMaterialSettings(mainCamera);
                goList[curSelectedGo]->material->Init(goList[curSelectedGo]->material->matSettings);
              }
              if(ImGui::MenuItem("Light", NULL, false, goList[curSelectedGo]->light == nullptr))
              {
                goList[curSelectedGo]->AddComponent<SE::SLight>();
                goList[curSelectedGo]->light->Init(SE::SLightInfo(kSun, glm::vec3(), glm::vec3()));
              }

              if (ImGui::MenuItem("Script", NULL, false, goList[curSelectedGo]->script == nullptr))
              {
                showScriptCreator = true;
              }
            }
            else { showErrorNotGOSelected = true; }
            ImGui::EndMenu();
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
          ImGui::MenuItem("Show Debug", NULL, &showDebugInfo);
          ImGui::MenuItem("Show Console", NULL, &showConsole);
          ImGui::MenuItem("Hide GUI (F12)", NULL, &hideGUI);
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
          if (ImGui::MenuItem("Serious Model Importer", NULL, false))
          {
            STARTUPINFO si;
            PROCESS_INFORMATION pi;

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            if (!CreateProcess(NULL,   // No module name (use command line)
              "..\\bin\\SModelImporter.exe",        // Command line
              NULL,           // Process handle not inheritable
              NULL,           // Thread handle not inheritable
              FALSE,          // Set handle inheritance to FALSE
              0,              // No creation flags
              NULL,           // Use parent's environment block
              NULL,           // Use parent's starting directory 
              &si,            // Pointer to STARTUPINFO structure
              &pi)           // Pointer to PROCESS_INFORMATION structure
              )
            {
              SDebug::GetInstance()->LogError("The program does not have enough permissions or the file can't be found");
            }
          }
          ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
      }

      //Show debug from the component cache
      SE::SComponentCache::GetInstance()->ShowGui();
      ImGui::Begin("Component debug");
      {
        ImGui::Text("Game Objects:%i", goList.size());
        ImGui::Separator();
      }
      ImGui::End();

      glm::vec2 windowSize = renderManRef->GetWindowSize();

      //Scene viewer
      ImGui::SetNextWindowPos(ImVec2(windowSize.x - windowSize.x / 5, 19));
      ImGui::SetNextWindowSize(ImVec2(windowSize.x / 5, windowSize.y / 2));
      ImGui::Begin("Hierarchy", &show, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
      ImGui::SetNextTreeNodeOpen(true);
      if (ImGui::TreeNode(""))
      {
        for (unsigned int i = 0; i < goList.size(); i++)
        {
          //Build the name
          std::string n = std::to_string(i);
          n += ":";
          n += goList[i]->myName.c_str();

          if (ImGui::Selectable(n.c_str(), curSelectedGo == i))
          {
            curSelectedGo = i;
          }
        }
        ImGui::TreePop();
      }
      ImGui::End();

      //Inspector
      ImGui::SetNextWindowPos(ImVec2(windowSize.x - windowSize.x / 5, windowSize.y / 2 + 19));
      ImGui::SetNextWindowSize(ImVec2(windowSize.x / 5, windowSize.y / 2 - 19));
      ImGui::Begin("Inspector", &show, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
      if (curSelectedGo != -1)
      {
        ImGui::Text(goList[curSelectedGo]->myName.c_str());
        ImGui::SameLine();
        if (ImGui::Button("Change name"))
        {
          showGONameEditor = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete GameObject"))
        {
          DeleteGameObject(curSelectedGo);
        }
        ImGui::Separator();
        //Show transformations
        if (curSelectedGo != -1)
        {
          ImGui::BeginChild("");
          ImGui::Text("Transform");
          {
            glm::vec3 posTrans = *goList[curSelectedGo]->transform->GetPosition();
            ImGui::InputFloat3("Position", glm::value_ptr(posTrans), 1);
            goList[curSelectedGo]->transform->SetPosition(posTrans);

            glm::vec3 rotTrans = *goList[curSelectedGo]->transform->GetRotation();
            ImGui::InputFloat3("Rotation", glm::value_ptr(rotTrans), 1);
            goList[curSelectedGo]->transform->SetRotation(rotTrans);

            glm::vec3 scaleTrans = *goList[curSelectedGo]->transform->GetScale();
            ImGui::InputFloat3("Scale", glm::value_ptr(scaleTrans), 1);
            goList[curSelectedGo]->transform->SetScale(scaleTrans);
          }
          ImGui::EndChild();

          //Scripts
          //If there is a script attached to the GameObject
          if (goList[curSelectedGo]->script != nullptr)
          {
            ImGui::BeginChild("");
            ImGui::Separator();
            ImGui::Text("Script");
            if (ImGui::Button("Edit Script"))
            {
              //Here we will load the script text
              if (curSelectedGo != -1)
              {
                strcpy(scriptText, goList[curSelectedGo]->script->GetScriptText());
                showScriptEditor = true;
              }
            }
            ImGui::SameLine();
            if (ImGui::Button("Remove Script"))
            {
              goList[curSelectedGo]->RemoveComponent<SScript>();
            }
            ImGui::EndChild();
          }

          //Show material
          if (goList[curSelectedGo]->material != nullptr)
          {
            ImGui::BeginChild("");
            ImGui::Separator();
            ImGui::Text("Material");
            goList[curSelectedGo]->material->matSettings->ShowGui();
            ImGui::EndChild();
          }

          //Show light gui
          if (goList[curSelectedGo]->light != nullptr)
          {
            ImGui::BeginChild("");
            ImGui::Separator();
            ImGui::Text("Light");
            goList[curSelectedGo]->light->ShowGui();
            ImGui::EndChild();
          }
        }
      }
      ImGui::End();

      //Debug info
      if (showDebugInfo)
      {
        ImGui::SetNextWindowPos(ImVec2(0, windowSize.y - windowSize.y / 5));
        ImGui::SetNextWindowSize(ImVec2(windowSize.x / 4, windowSize.y / 5));
        ImGui::Begin("Debug Info", &showDebugInfo, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
        {
          ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
          ImGui::Text("Mouse pos: %.1f, %.1f", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
          ImGui::Text("DrawCalls: %i", *renderManRef->GetDrawCalls());
          ImGui::Text("DrawTris: %i", *renderManRef->GetDrawTriangles());
          ImGui::End();
        }
      }

      //Console
      //TO DO: The draw function can't be called from anywhere
      if (showConsole)
      {
        SDebug::GetInstance()->Draw(ImVec2(windowSize.x / 4, windowSize.y - windowSize.y / 5), ImVec2(windowSize.x / 2 + windowSize.x / 20, windowSize.y / 5), showConsole);
      }

      //Script Text Editor
      if (showScriptEditor)
      {
        if (goList[curSelectedGo]->script != nullptr)
        {
          ImGui::Begin("Script Editor", &showScriptEditor, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
          {
            // Menu
            if (ImGui::BeginMenuBar())
            {
              if (ImGui::BeginMenu("File"))
              {
                //Save the current script modified
                if (ImGui::MenuItem("Save Script", NULL, false))
                {
                  goList[curSelectedGo]->script->SaveScriptText(scriptText);
                }
                if (ImGui::MenuItem("Exit", NULL, false)) { showScriptEditor = false; }
                ImGui::EndMenu();
              }
              ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PopStyleVar();
            ImGui::InputTextMultiline("##source", scriptText, IM_ARRAYSIZE(scriptText), ImVec2(windowSize.x / 2, windowSize.y / 3), ImGuiInputTextFlags_AllowTabInput);
          }
          ImGui::End();
        }
      }

      //Mesh Component
      if (showMeshCreator)
      {
        mainCamera->SetCameraLock(true);
        ImGui::Begin("Edit Creator", &showMeshCreator, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize);
        {
          ImGui::Text("This mesh has to be exported to Serious Model(.sm) with the Serious Model Importer.");
          ImGui::Text("Engine Imported path: ('data/imported/...')");
          ImGui::Text("Path: "); ImGui::SameLine();
          static char meshPath[100] = ""; ImGui::InputText("", meshPath, 100, ImGuiInputTextFlags_AllowTabInput);
          if (ImGui::Button("Add Mesh"))
          {
            char folder[] = "../data/imported/";
            char *fullPath = new char[std::strlen(folder) + std::strlen(meshPath) + 1];
            std::strcpy(fullPath, folder);
            std::strcat(fullPath, meshPath);

            SE::SMeshConstructor meshConstructor(SE::kTriangles, fullPath);

            if (goList[curSelectedGo]->AddComponent<SE::SMesh>(meshConstructor))
            {
              goList[curSelectedGo]->AddComponent<SE::SMaterial>();
              goList[curSelectedGo]->material->Init(defaultMatSet);
            }
            memset(&meshPath[0], 0, sizeof(meshPath));
            showMeshCreator = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) { showMeshCreator = false; }
        }
        ImGui::End();
      }

      //Script Text Creator
      if (showScriptCreator)
      {
        if (goList[curSelectedGo]->script == nullptr)
        {
          ImGui::Begin("Script Creator", &showScriptCreator, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize);
          {
            ImGui::Text("This model will be saved into the scripts folder.");
            ImGui::Text("Engine Script path: ('data/scripts/...')");
            ImGui::Text("You could have your own subfolders.");

            ImGui::Separator();
            ImGui::Text("Path to the script subfolder:"); ImGui::SameLine();
            static char scriptSubpath[100] = ""; ImGui::InputText("", scriptSubpath, 100, ImGuiInputTextFlags_AllowTabInput);
            if (ImGui::Button("Add Script"))
            {
              char folder[] = "../data/scripts/";
              char extension[] = ".lua";
              char *file = new char[std::strlen(scriptSubpath) + std::strlen(extension) + 1];
              std::strcpy(file, scriptSubpath);
              std::strcat(file, extension);

              char *fullPath = new char[std::strlen(folder) + std::strlen(file) + 1];
              std::strcpy(fullPath, folder);
              std::strcat(fullPath, file);

              goList[curSelectedGo]->AddComponent<SScript>();
              goList[curSelectedGo]->script->Init(fullPath);
              strcpy(scriptText, goList[curSelectedGo]->script->GetScriptText());

              memset(&scriptSubpath[0], 0, sizeof(scriptSubpath));
              showScriptEditor = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) { showScriptCreator = false; }
          }
          ImGui::End();
        }
        else
        {
          showScriptCreator = false;
        }
      }

      //GameObject Creator
      if (showGameObjectCreator)
      {
        ImGui::Begin("GameObject Creator", &showGameObjectCreator, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize);
        {
          ImGui::Text("Name:"); ImGui::SameLine();
          static char name[100] = ""; ImGui::InputText("", name, 100, ImGuiInputTextFlags_AllowTabInput);
          if (ImGui::Button("Create"))
          {
            if (name[0] == '\0')
            {
              std::string tmp = "GameObject";
              AddGameObject(tmp.c_str());
            }
            else
            {
              AddGameObject(name);
              memset(&name[0], 0, sizeof(name));
            }
            curSelectedGo = goList.size() - 1;
            showGameObjectCreator = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) { showGameObjectCreator = false; }
        }
        ImGui::End();
      }

      if (showErrorNotGOSelected)
      {
        ImGui::Begin("Error", &showErrorNotGOSelected, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize);
        {
          ImGui::Text("You have to select a GameObject which you want to add a component");
          if (ImGui::Button("OK")) { showErrorNotGOSelected = false; }
        }
        ImGui::End();
      }

      if (showGONameEditor)
      {
        mainCamera->SetCameraLock(true);
        ImGui::Begin("Edit Name", &showGONameEditor, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize);
        {
          ImGui::Text("New name:"); ImGui::SameLine();
          static char newName[100] = ""; ImGui::InputText("", newName, 100, ImGuiInputTextFlags_AllowTabInput);
          if (ImGui::Button("Change"))
          {
            goList[curSelectedGo]->myName = newName;
            memset(&newName[0], 0, sizeof(newName));
            showGONameEditor = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) { showGONameEditor = false; }
        }
        ImGui::End();
      }

      if (showSceneSaver)
      {
        ImGui::Begin("Save Scene", &showSceneSaver, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize);
        {
          ImGui::Text("This scene will be saved into the Scene folder.");
          ImGui::Text("Engine Scene path: ('data/scenes/...')");
          ImGui::Text("You could have your own subfolders.");

          ImGui::Separator();
          ImGui::Text("Path to the scene subfolder:"); ImGui::SameLine();
          static char sceneSubpath[100] = ""; ImGui::InputText("", sceneSubpath, 100, ImGuiInputTextFlags_AllowTabInput);
          if (ImGui::Button("Save Scene"))
          {
            char folder[] = "../data/scenes/";
            char extension[] = ".xml";
            char *file = new char[std::strlen(sceneSubpath) + std::strlen(extension) + 1];
            std::strcpy(file, sceneSubpath);
            std::strcat(file, extension);

            char *fullPath = new char[std::strlen(folder) + std::strlen(file) + 1];
            std::strcpy(fullPath, folder);
            std::strcat(fullPath, file);

            SSceneParser::GetInstance()->SaveScene(goList, fullPath);

            memset(&sceneSubpath[0], 0, sizeof(sceneSubpath));
            showSceneSaver = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) { showSceneSaver = false; }
        }
        ImGui::End();
      }

      if (showSceneLoader)
      {
        ImGui::Begin("Load Scene", &showSceneLoader, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize);
        {
          ImGui::Text("This scene will be loaded from the Scene folder.");
          ImGui::Text("Engine Scene path: ('data/scenes/...')");

          ImGui::Separator();
          ImGui::Text("Path to the scene subfolder:"); ImGui::SameLine();
          static char sceneLSubpath[100] = ""; ImGui::InputText("", sceneLSubpath, 100, ImGuiInputTextFlags_AllowTabInput);
          if (ImGui::Button("Load Scene"))
          {
            char folder[] = "../data/scenes/";
            char *fullPath = new char[std::strlen(folder) + std::strlen(sceneLSubpath) + 1];
            std::strcpy(fullPath, folder);
            std::strcat(fullPath, sceneLSubpath);

            SSceneParser::GetInstance()->LoadScene(goList, fullPath, mainCamera->GetPosition());

            memset(&sceneLSubpath[0], 0, sizeof(sceneLSubpath));
            showSceneLoader = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) { showSceneLoader = false; }
        }
        ImGui::End();
      }

      if (showScriptEditor || showScriptCreator || showGameObjectCreator || showErrorNotGOSelected || showGONameEditor || showMeshCreator || showSceneSaver || showSceneLoader)
      {
        mainCamera->SetCameraLock(true);
      }
      else { mainCamera->SetCameraLock(false); }
    }
  }

  ///////////////////////////////////////////////////////////////////

  int SScene::AddGameObject(const char* name)
  {
    int cInd = AddGameObject();
    goList[cInd]->Init(name);
    goList[cInd]->AddComponent<SE::STransform>();
    goList[cInd]->transform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    return cInd;
  }

  ///////////////////////////////////////////////////////////////////
}