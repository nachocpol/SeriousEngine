#include "SModelImporter/SMIScene.h"
#include "Engine/SDebug.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <fstream>

namespace SE
{
  ///////////////////////////////////////////////////////////////////

  SMIScene::SMIScene()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SMIScene::~SMIScene()
  {

  }

  ///////////////////////////////////////////////////////////////////

  void SMIScene::Render()
  {
    mainCamera->SetCameraLock(true);
    mainCamera->SetImporter();

    //Upload the draw data to the uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, dDataBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetView()));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(*mainCamera->GetProjection()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    displayList->SetShadowPass(true);
    for (unsigned int i = 0; i<lights.size(); i++)
    {
      displayList->SetCurLightMatrix(lights[i]->GetLightMatrix());
      lights[i]->EnableLightShadow();
      displayList->RunDl();
      lights[i]->DisableLightShadow();
    }
    displayList->SetShadowPass(false);

    //Basic rendering the scene
    displayList->SetCurrentLights(lights);
    displayList->SetCamera(mainCamera);
    displayList->RunDl();

    displayList->CleanDl();
    lights.clear();

    //Render the gui
    RenderGUI();
  }

  ///////////////////////////////////////////////////////////////////

  void SMIScene::RenderGUI()
  {
    static bool show = true;
    static bool showLoadModel = false;
    static bool showExportModel = false;
    static bool showDebugInfo = true;
    static bool showConsole = true;

    //ImGui::ShowTestWindow(&show);

    // Menu
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Load Model", NULL, false)) { showLoadModel = true; }
        if (ImGui::MenuItem("Export to SModel", NULL, false)) { showExportModel = true; }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Settings"))
      {
        ImGui::MenuItem("Show Debug", NULL, &showDebugInfo);
        ImGui::MenuItem("Show Console", NULL, &showConsole);
        ImGui::MenuItem("Hide GUI", NULL, &showConsole);
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }

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
      //Show transformations
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

      //Show material
      if (goList[curSelectedGo]->material != nullptr)
      {
        ImGui::BeginChild("");
        ImGui::Separator();
        ImGui::Text("Material");
        //If it has a material show the material gui
        goList[curSelectedGo]->material->matSettings->ShowGui();
        ImGui::EndChild();
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
        ImGui::Text("FPS:%.2f", ImGui::GetIO().Framerate);
        ImGui::Text("Mouse pos:%.1f,%.1f", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::Text("DrawCalls:%i", *renderManRef->GetDrawCalls());
        ImGui::Text("DrawTris:%i", *renderManRef->GetDrawTriangles());
        ImGui::End();
      }
    }

    //Console
    //TO DO: The draw function can't be called from anywhere
    if (showConsole)
    {
      SDebug::GetInstance()->Draw(ImVec2(windowSize.x / 4, windowSize.y - windowSize.y / 5), ImVec2(windowSize.x / 2 + windowSize.x / 20, windowSize.y / 5), showConsole);
    }

    //Load model Window
    if (showLoadModel)
    {
      //ImGui::SetNextWindowSize(ImVec2(windowSize.x / 3, windowSize.y / 5));
      ImGui::Begin("Load Model", &showLoadModel, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize);
      {
        ImGui::Text("This model importer allows OBJ, DAE, FBX and BLEND.");
        ImGui::Separator();
        ImGui::Checkbox("Override UVs", &overrideUvs);
        ImGui::Text("Path to the file:"); ImGui::SameLine();
        static char buf1[100] = ""; ImGui::InputText("", buf1, 100, ImGuiInputTextFlags_AllowTabInput);
        if (ImGui::Button("Load File"))
        {
          if (LoadModel(buf1))
          {
            showLoadModel = false;
          }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) { showLoadModel = false; }
      }
      ImGui::End();
    }

    //Export model Window
    if (showExportModel)
    {
      //ImGui::SetNextWindowSize(ImVec2(windowSize.x / 3, windowSize.y / 5));
      ImGui::Begin("Export to Serious Model", &showExportModel, ImVec2(0, 0), 0.6f, ImGuiWindowFlags_AlwaysAutoResize);
      {
        ImGui::Text("This model will be saved into the imported folder.");
        ImGui::Text("Engine Imported path: ('data/imported/...')");
        ImGui::Text("You could have your own subfolders.");

        ImGui::Separator();
        ImGui::Text("Path to the imported subfolder:"); ImGui::SameLine();
        static char buf2[100] = ""; ImGui::InputText("", buf2, 100, ImGuiInputTextFlags_AllowTabInput);
        if (ImGui::Button("Export model")) { ExportSModel(buf2, *data); showExportModel = false; }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) { showExportModel = false; }
      }
      ImGui::End();
    }

    if (showLoadModel || showExportModel)
    {
      mainCamera->SetCameraLock(true);
    }
    else { mainCamera->SetCameraLock(false); }
  }

  ///////////////////////////////////////////////////////////////////

  bool SMIScene::LoadModel(const char *path)
  {
    if (goList.size() > 1)
    {
      DeleteGameObject(modelID);
    }

    if (data != nullptr)
    {
      delete data;
      data = nullptr;
    }

    data = new SE::SBinaryModelData();
    ASSIMPLoad(path);

    if (data->vertexSize != 0)
    {
      SE::SMeshConstructorProcedural meshConstructor(SE::kTriangles, *data);

      modelID = AddGameObject();
      goList[modelID]->Init("Imported Mesh");
      goList[modelID]->AddComponent<SE::STransform>();
      goList[modelID]->transform->SetPosition(glm::vec3(0, -1, 0));
      goList[modelID]->transform->Scale(glm::vec3(1.0f));
      goList[modelID]->transform->Rotate(glm::vec3(0.0f, 0.0f, 0.0f));
      goList[modelID]->AddComponent<SE::SMesh>();
      goList[modelID]->mesh->Init(meshConstructor);

      goList[modelID]->AddComponent<SE::SMaterial>();
      //Material settings
      goList[modelID]->material->Init(defaultMatSet);

      return true;
    }
    return false;
  }

  ///////////////////////////////////////////////////////////////////

  void SMIScene::ASSIMPLoad(const char* p)
  {
    curUvCnt = 0;
    numVertex = 0;
    numUV = 0;
    numTangents = 0;

    //Load scene from file
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(p,
      aiProcess_Triangulate |
      aiProcess_FlipUVs |
      aiProcess_GenNormals |
      aiProcess_CalcTangentSpace);

    //Check error
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      SE::SDebug::GetInstance()->LogError("Could not load this model!: %s", importer.GetErrorString());
    }

    std::vector<float> lPosition;
    std::vector<float> lNormals;
    std::vector<float> lUvs;
    std::vector<float> lTangents;
    std::vector<unsigned int> lIndices;

    //Vertex data
    for (unsigned int m = 0; m < scene->mNumMeshes; m++)
    {
      for (GLuint i = 0; i < scene->mMeshes[m]->mNumVertices; i++)
      {
        // Positions
        lPosition.push_back(scene->mMeshes[m]->mVertices[i].x);
        lPosition.push_back(scene->mMeshes[m]->mVertices[i].y);
        lPosition.push_back(scene->mMeshes[m]->mVertices[i].z);

        numVertex += 3;

        // Normals
        lNormals.push_back(scene->mMeshes[m]->mNormals[i].x);
        lNormals.push_back(scene->mMeshes[m]->mNormals[i].y);
        lNormals.push_back(scene->mMeshes[m]->mNormals[i].z);

        // Tangents
        if (scene->mMeshes[m]->mTangents)
        {
          lTangents.push_back(scene->mMeshes[m]->mTangents[i].x);
          lTangents.push_back(scene->mMeshes[m]->mTangents[i].y);
          lTangents.push_back(scene->mMeshes[m]->mTangents[i].z);
          numTangents += 3;
        }

        // Texture Coordinates
        if (scene->mMeshes[m]->mTextureCoords[0] || overrideUvs)
        {
          if (overrideUvs)
          {
            if (curUvCnt == 0)
            {
              lUvs.push_back(0);
              lUvs.push_back(0);
            }
            else if (curUvCnt == 1)
            {
              lUvs.push_back(1);
              lUvs.push_back(0);
            }
            else if (curUvCnt == 2)
            {
              lUvs.push_back(1);
              lUvs.push_back(1);
            }
            else if (curUvCnt == 3)
            {
              lUvs.push_back(0);
              lUvs.push_back(1);
            }
            curUvCnt += 1;
            if (curUvCnt > 3)
            {
              curUvCnt = 0;
            }
          }
          else
          {
            lUvs.push_back(scene->mMeshes[m]->mTextureCoords[0][i].x);
            lUvs.push_back(scene->mMeshes[m]->mTextureCoords[0][i].y);
          }
        }
        else
        {
          lUvs.push_back(0);
          lUvs.push_back(0);
        }
        numUV += 2;
      }

      //Indices
      for (unsigned int f = 0; f < scene->mMeshes[m]->mNumFaces; f++)
      {
        aiFace face = scene->mMeshes[m]->mFaces[f];
        for (unsigned int k = 0; k < face.mNumIndices; k++)
        {
          lIndices.push_back(face.mIndices[k]);
        }
      }
    }

    //Actual data to be exported
    data->vertexSize = numVertex;
    data->normalSize = numVertex;
    data->UVSize = numUV;
    data->tangentsSize = numTangents;
    data->indicesSize = lIndices.size();

    data->indices = lIndices;
    data->vertices = lPosition;
    data->vertices.insert(data->vertices.end(), lUvs.begin(), lUvs.end());
    data->vertices.insert(data->vertices.end(), lNormals.begin(), lNormals.end());
    data->vertices.insert(data->vertices.end(), lTangents.begin(), lTangents.end());
  }
  ///////////////////////////////////////////////////////////////////

  void SMIScene::ExportSModel(const char *path, SE::SBinaryModelData &dat)
  {
    char folder[] = "../data/imported/";
    char extension[] = ".sm";
    char *file = new char[std::strlen(path) + std::strlen(extension) + 1];
    std::strcpy(file, path);
    std::strcat(file, extension);

    char *fullPath = new char[std::strlen(folder) + std::strlen(file) + 1];
    std::strcpy(fullPath, folder);
    std::strcat(fullPath, file);

    std::ofstream writeFile;
    writeFile.open(fullPath, std::ios::out | std::ios::binary);
    if (writeFile)
    {
      if (!dat.vertices.empty())
      {
        writeFile.write(reinterpret_cast<char*>(&dat.vertexSize), sizeof(unsigned int));
        writeFile.write(reinterpret_cast<char*>(&dat.normalSize), sizeof(unsigned int));
        writeFile.write(reinterpret_cast<char*>(&dat.UVSize), sizeof(unsigned int));
        writeFile.write(reinterpret_cast<char*>(&dat.tangentsSize), sizeof(unsigned int));
        writeFile.write(reinterpret_cast<char*>(&dat.indicesSize), sizeof(unsigned int));
        writeFile.write(reinterpret_cast<char*>(&dat.vertices[0]), ((dat.vertexSize + dat.UVSize + dat.normalSize + dat.tangentsSize) * sizeof(float)));
        writeFile.write(reinterpret_cast<char*>(&dat.indices[0]), dat.indicesSize * sizeof(unsigned int));
      }
    }
    else
    {
      std::cout << "Can't write in: " << path << std::endl;
    }
    writeFile.close();
    delete[] file;
    delete[] fullPath;
  }
  
  ///////////////////////////////////////////////////////////////////
}