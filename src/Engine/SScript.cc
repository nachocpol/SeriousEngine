#include "Engine/SScript.h"
#include "Engine/SGameObject.h"
#include "Engine/SRenderManager.h"
#include "Engine/SDebug.h"

#include <iostream>
#include <fstream>

using namespace SE;
using namespace luabridge;

const char defaultText[] = "Start = function()\n-- This function is being called at the start\nend\nUpdate = function()\n-- This function is called once per frame\nend";

///////////////////////////

void DebugLog(const std::string& log)
{
  SDebug::GetInstance()->Log(log.c_str());
}

///////////////////////////

float GetDeltaTime()
{
  return SRenderManager::GetInstance()->GetDeltaTimeS();
}

//////////////////////////

float GetTime()
{
  return SRenderManager::GetInstance()->GetTime();
}

///////////////////////////

bool fileExist(const std::string& name) {
  if (FILE *file = fopen(name.c_str(), "r")) {
    fclose(file);
    return true;
  }
  else {
    return false;
  }
}

///////////////////////////

SScript::SScript()
{

}

SScript::SScript(SGameObject *go)
{
  gameObject = go;

}

///////////////////////////

SScript::~SScript()
{
  delete scriptTextBuffer;
  scriptTextBuffer = nullptr;
  path = nullptr;
}

///////////////////////////

void SScript::Translate(glm::vec3 p)
{
   gameObject->transform->Translate(p);
}

///////////////////////////

void SScript::Rotate(glm::vec3 r)
{
  gameObject->transform->Rotate(r);
}

///////////////////////////

void SScript::Scale(glm::vec3 s)
{
  gameObject->transform->Scale(s);
}

///////////////////////////

void SScript::SetPosition(glm::vec3 p)
{
  gameObject->transform->SetPosition(p);
}

///////////////////////////

void SScript::SetRotation(glm::vec3 r)
{
  gameObject->transform->SetRotation(r);
}

///////////////////////////

void SScript::SetScale(glm::vec3 r)
{
  gameObject->transform->SetScale(r);
}

///////////////////////////

glm::vec3 SScript::GetPosition()
{
  return *gameObject->transform->GetPosition();
}

///////////////////////////

glm::vec3 SScript::GetRotation()
{
  return *gameObject->transform->GetRotation();
}

///////////////////////////

glm::vec3 SScript::GetScale()
{
  return *gameObject->transform->GetScale();
}

///////////////////////////


void SScript::Init(const char *filepath)
{
  path = filepath;

  if (!fileExist(filepath))
  {
    std::ofstream writeFile;
    writeFile.open(filepath);

    writeFile << defaultText;
    writeFile.close();
  }

  L = luaL_newstate();
  luaL_openlibs(L);
  
  //GLM vec3
  getGlobalNamespace(L)
    .beginNamespace("glm")
      .beginClass<glm::vec3>("vec3")
        .addConstructor<void(*)(float x, float y, float z)>()
        .addData("x", &glm::vec3::x)
        .addData("y", &glm::vec3::y)
        .addData("z", &glm::vec3::z)
      .endClass()
    .endNamespace();

  //Engine functions
  getGlobalNamespace(L)
    .beginNamespace("Engine")
      .addFunction("DebugLog", DebugLog)
      .addFunction("GetDeltaTime", GetDeltaTime)
      .addFunction("GetTime", GetTime)
    .endNamespace();

  //GameObjects functions
  getGlobalNamespace(L)
    .beginClass<SScript>("SScriptComponent")
      .addConstructor<void(*)()>()
      .addFunction("Translate", &SScript::Translate)
      .addFunction("Rotate", &SScript::Rotate)
      .addFunction("Scale", &SScript::Scale)
      .addFunction("SetPosition", &SScript::SetPosition)
      .addFunction("SetRotation", &SScript::SetRotation)
      .addFunction("SetScale", &SScript::SetScale)
    .endClass();

  //Copy of this script passed, Lua lifetime.
  push(L, this);                    
  lua_setglobal(L, "this");

  Reload();
}

///////////////////////////

void SScript::Update()
{
  if (!wrongCode)
  {
    try 
    {
      LuaRef update = getGlobal(L, "Update");
      update();
    }
    catch (LuaException &e) 
    {
      wrongCode = true;
    }
  }
}

///////////////////////////

void SScript::Reload()
{
  if (luaL_dofile(L, path.c_str())) { SDebug::GetInstance()->LogError(lua_tostring(L, -1)); }

  try 
  {
    LuaRef start = getGlobal(L, "Start");
    start();
    if (wrongCode) { wrongCode = false; }
  }
  catch (LuaException &e) 
  {
    SDebug::GetInstance()->LogError(e.what());
    wrongCode = true;
  }
  try
  {
    LuaRef update = getGlobal(L, "Update");
    update();
    if (wrongCode) { wrongCode = false; }
  }
  catch (LuaException &e)
  {
    SDebug::GetInstance()->LogError(e.what());
    wrongCode = true;
  }
}

///////////////////////////

void SScript::LoadScriptText()
{
  int array_size = 1024 * 16;
  int position = 0;

  if (scriptTextBuffer != nullptr)
  {
    delete scriptTextBuffer;
    scriptTextBuffer = nullptr;
  }
  scriptTextBuffer = new char[array_size];

  std::ifstream fin(path);
  if (fin.is_open())
  {
    while (!fin.eof() && position < array_size)
    {
      fin.get(scriptTextBuffer[position]);
      position++;
    }
    scriptTextBuffer[position - 1] = '\0';
    fin.close();
  }
}

///////////////////////////

char *SScript::GetScriptText()
{
  LoadScriptText();
  return scriptTextBuffer;
}

///////////////////////////

void SScript::SaveScriptText(const char *text)
{
  std::ofstream out(path);
  if (out.is_open())
  {
    out << text;
    out.close();
  }

  Reload();
}

///////////////////////////