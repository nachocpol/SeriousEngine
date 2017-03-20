#include "Engine/SSceneParser.h"
#include "Engine/STypes.h"
#include "Engine/SDebug.h"
#include "Engine/SResourceManager.h"

#include "tinyxml2.h"
#include "glm.hpp"

using namespace tinyxml2;

namespace SE
{
  struct MaterialParser
  {
    const char* vsPath;
    const char* fsPath;
    const char* diffusePath, *glossPath, *specularPath, *normalPath;
  };

  struct TransformParser
  {
    glm::vec3 position, rotation, scale;
  };

  struct MeshParser
  {
    const char* path;
  };

  struct ScriptParser
  {
    const char* path;
  };

  struct GOParser
  {
    const char* name;
    TransformParser* transform;
    MeshParser* mesh;
    MaterialParser* material;
    SLightInfo* light;
    ScriptParser* script;
  };

  ///////////////////////////////////////////////////////////////////

  SSceneParser::SSceneParser()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SSceneParser::~SSceneParser()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SSceneParser* SSceneParser::GetInstance()
  {
    static SSceneParser instance;
    return &instance;
  }

  ///////////////////////////////////////////////////////////////////

  void SSceneParser::SaveScene(const std::vector<SGameObject *> &list, const char* savePath)
  {
    XMLDocument doc;

    XMLNode *pRoot = doc.NewElement("Root");
    doc.InsertFirstChild(pRoot);

    for (int i = 0; i < list.size(); i++)
    {
      XMLElement *GO = doc.NewElement("GameObject");
      GO->SetAttribute("Name", list[i]->myName.c_str());
      pRoot->InsertEndChild(GO);

      XMLElement *transform = doc.NewElement("Transform");
      GO->InsertEndChild(transform);

      glm::vec3 pos = *list[i]->transform->GetPosition();
      XMLElement *position = doc.NewElement("Position");
      position->SetAttribute("X", pos.x);
      position->SetAttribute("Y", pos.y);
      position->SetAttribute("Z", pos.z);
      transform->InsertEndChild(position);

      glm::vec3 rot = *list[i]->transform->GetRotation();
      XMLElement *rotation = doc.NewElement("Rotation");
      rotation->SetAttribute("X", rot.x);
      rotation->SetAttribute("Y", rot.y);
      rotation->SetAttribute("Z", rot.z);
      transform->InsertEndChild(rotation);

      glm::vec3 _scale = *list[i]->transform->GetScale();
      XMLElement *scale = doc.NewElement("Scale");
      scale->SetAttribute("X", _scale.x);
      scale->SetAttribute("Y", _scale.y);
      scale->SetAttribute("Z", _scale.z);
      transform->InsertEndChild(scale);

      if (list[i]->mesh != nullptr)
      {
        XMLElement *mesh = doc.NewElement("Mesh");
        mesh->SetAttribute("Path", list[i]->mesh->GetPath());
        GO->InsertEndChild(mesh);
      }

      if (list[i]->material != nullptr)
      {
        XMLElement *material = doc.NewElement("Material");
        GO->InsertEndChild(material);

        SPhysicalMatSet *matSet = (SPhysicalMatSet*)list[i]->material->matSettings;
        XMLElement *shader = doc.NewElement("Shader");
        shader->SetAttribute("VertexPath", matSet->vsp);
        shader->SetAttribute("FragmentPath", matSet->fsp);
        material->InsertEndChild(shader);

        XMLElement *texture = doc.NewElement("Texture");
        texture->SetAttribute("DiffusePath", matSet->diffPath.c_str());
        texture->SetAttribute("GlossPath", matSet->glossPath.c_str());
        texture->SetAttribute("SpecularPath", matSet->specPath.c_str());
        texture->SetAttribute("NormalPath", matSet->normPath.c_str());
        material->InsertEndChild(texture);
      }

      if (list[i]->light != nullptr)
      {
        SLightInfo* lightInfo = &list[i]->light->GetLightInfo();
        XMLElement* light = doc.NewElement("Light");
        if (lightInfo->lType == SE::kSun)
        {
          light->SetAttribute("Type", "Directional");
        }
        else if (lightInfo->lType == SE::kSpotLight)
        {
          light->SetAttribute("Type", "Point");
        }
        GO->InsertEndChild(light);

        XMLElement *direction = doc.NewElement("Direction");
        direction->SetAttribute("X", lightInfo->lDir.x);
        direction->SetAttribute("Y", lightInfo->lDir.y);
        direction->SetAttribute("Z", lightInfo->lDir.z);
        light->InsertEndChild(direction);

        XMLElement *color = doc.NewElement("Color");
        color->SetAttribute("R", lightInfo->lCol.x);
        color->SetAttribute("G", lightInfo->lCol.y);
        color->SetAttribute("B", lightInfo->lCol.z);
        light->InsertEndChild(color);

        XMLElement *data = doc.NewElement("Data");
        data->SetAttribute("Near", lightInfo->lNear);
        data->SetAttribute("Far", lightInfo->lFar);
        data->SetAttribute("Size", lightInfo->lSize);
        light->InsertEndChild(data);
      }

      if (list[i]->script != nullptr)
      {
        XMLElement *script = doc.NewElement("Script");
        script->SetAttribute("Path", list[i]->script->GetPath());
        GO->InsertEndChild(script);
      }

      XMLError eResult = doc.SaveFile(savePath);
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SSceneParser::LoadScene(std::vector<SGameObject *> &list, const char* loadPath, glm::vec3 *camPos)
  {
    //Delete previous scene
    do
    {
      list[0]->DeleteGameObject();
      list.erase(list.begin());
    } while (list.size() > 0);

    // Delete all the resources
    //SE::SResourceManager::GetInstance()->ClearResources();

    std::vector <GOParser*> result;

    XMLDocument doc;
    XMLError eResult = doc.LoadFile(loadPath);
    if (eResult == 0)
    {
      XMLNode *pRoot = doc.FirstChild();
      XMLElement *goList = pRoot->FirstChildElement("GameObject");

      while (goList != nullptr)
      {
        result.push_back(new GOParser);
        int cInd = result.size() - 1;

        result[cInd]->name = goList->Attribute("Name");

        result[cInd]->transform = new TransformParser;

        XMLElement *transform = goList->FirstChildElement("Transform");
        XMLElement *position = transform->FirstChildElement("Position");
        position->QueryFloatAttribute("X", &result[cInd]->transform->position.x);
        position->QueryFloatAttribute("Y", &result[cInd]->transform->position.y);
        position->QueryFloatAttribute("Z", &result[cInd]->transform->position.z);

        XMLElement *rotation = transform->FirstChildElement("Rotation");
        rotation->QueryFloatAttribute("X", &result[cInd]->transform->rotation.x);
        rotation->QueryFloatAttribute("Y", &result[cInd]->transform->rotation.y);
        rotation->QueryFloatAttribute("Z", &result[cInd]->transform->rotation.z);

        XMLElement *scale = transform->FirstChildElement("Scale");
        scale->QueryFloatAttribute("X", &result[cInd]->transform->scale.x);
        scale->QueryFloatAttribute("Y", &result[cInd]->transform->scale.y);
        scale->QueryFloatAttribute("Z", &result[cInd]->transform->scale.z);

        XMLElement *mesh = goList->FirstChildElement("Mesh");
        if (mesh != nullptr)
        {
          result[cInd]->mesh = new MeshParser;
          result[cInd]->mesh->path = mesh->Attribute("Path");
        }
        else { result[cInd]->mesh = nullptr; }

        XMLElement *material = goList->FirstChildElement("Material");
        if (material != nullptr)
        {
          result[cInd]->material = new MaterialParser;
          XMLElement *shader = material->FirstChildElement("Shader");
          result[cInd]->material->vsPath = shader->Attribute("VertexPath");
          result[cInd]->material->fsPath = shader->Attribute("FragmentPath");

          XMLElement *texture = material->FirstChildElement("Texture");
          result[cInd]->material->diffusePath = texture->Attribute("DiffusePath");
          result[cInd]->material->glossPath = texture->Attribute("GlossPath");
          result[cInd]->material->specularPath = texture->Attribute("SpecularPath");
          result[cInd]->material->normalPath = texture->Attribute("NormalPath");
        }
        else { result[cInd]->material = nullptr; }

        XMLElement *light = goList->FirstChildElement("Light");
        if (light != nullptr)
        {
          result[cInd]->light = new SLightInfo;

          if (strcmp(light->Attribute("Type"), "Directional") == 0)
          {
            result[cInd]->light->lType = SE::kSun;
          }
          else if (strcmp(light->Attribute("Type"), "Point") == 0)
          {
            result[cInd]->light->lType = SE::kSpotLight;
          }

          XMLElement *direction = light->FirstChildElement("Direction");
          direction->QueryFloatAttribute("X", &result[cInd]->light->lDir.x);
          direction->QueryFloatAttribute("Y", &result[cInd]->light->lDir.y);
          direction->QueryFloatAttribute("Z", &result[cInd]->light->lDir.z);

          XMLElement *color = light->FirstChildElement("Color");
          color->QueryFloatAttribute("R", &result[cInd]->light->lCol.x);
          color->QueryFloatAttribute("G", &result[cInd]->light->lCol.y);
          color->QueryFloatAttribute("B", &result[cInd]->light->lCol.z);

          XMLElement *data = light->FirstChildElement("Data");
          data->QueryFloatAttribute("Near", &result[cInd]->light->lNear);
          data->QueryFloatAttribute("Far", &result[cInd]->light->lFar);
          data->QueryFloatAttribute("Size", &result[cInd]->light->lSize);
        }
        else { result[cInd]->light = nullptr; }

        XMLElement *script = goList->FirstChildElement("Script");
        if (script != nullptr)
        {
          result[cInd]->script = new ScriptParser;
          result[cInd]->script->path = script->Attribute("Path");
        }
        else { result[cInd]->script = nullptr; }

        goList = goList->NextSiblingElement("GameObject");
      }

      for (int i = 0; i < result.size(); i++)
      {
        list.push_back(new SGameObject());
        int cInd = list.size() - 1;

		    list[cInd]->Init(result[i]->name);
        list[cInd]->AddComponent<SE::STransform>();
        list[cInd]->transform->SetPosition(result[i]->transform->position);
        list[cInd]->transform->SetRotation(result[i]->transform->rotation);
        list[cInd]->transform->SetScale(result[i]->transform->scale);

        if (result[i]->mesh != nullptr)
        {
          list[cInd]->AddComponent<SE::SMesh>(SE::SMeshConstructor(SE::kTriangles, result[i]->mesh->path));
        }

        if (result[i]->material != nullptr)
        {
          list[cInd]->AddComponent<SE::SMaterial>();
          SE::SPhysicalMatSet* MatSet = new SE::SPhysicalMatSet(result[i]->material->vsPath, result[i]->material->fsPath, result[i]->material->diffusePath, result[i]->material->glossPath, result[i]->material->specularPath, result[i]->material->normalPath);
          MatSet->refCamPos = camPos;
          list[cInd]->material->Init(MatSet);
        }

        if (result[i]->light != nullptr)
        {
          list[cInd]->AddComponent<SE::SLight>();
          list[cInd]->light->Init(*result[i]->light);
        }

        if (result[i]->script != nullptr)
        {
          list[cInd]->AddComponent<SE::SScript>();
          list[cInd]->script->Init(result[i]->script->path);
        }
      }

      result.clear();
    }
    else
    {
      SDebug::GetInstance()->LogError("Can't open the file located in %s", loadPath);
    }
  }
}

/*
----------------------------------------------------------

<Root>
  <GameObject Name="Pepe">
    <Transform>
      <Position X="0" Y="0.5" Z="0"/>
      <Rotation X="90" Y="0" Z="0"/>
      <Scale X="1" Y="1" Z="1"/>
    </Transform>
    <Mesh Path="../data/models/teapot.sm"/>
    <Material>
      <Shader VertexPath="" FragmentPath=""/>
      <Texture DiffusePath="" GlossPath="" SpecularPath="" NormalPath=""/>
    </Material>
    <Light Type="Sun">
      <Direction X="" Y="" Z=""/>
      <Color R="" G="" B=""/>
      <Data Near="" Far="" Size=""/>
    </Light>
    <Script Path=""/>
  </GameObject>
</Root>

----------------------------------------------------------
*/