/*
SScript.h David Parra
*/

#ifndef SSCRIPT
#define SSCRIPT

#include "glm.hpp"

#include "LuaBridge.h"

extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include <iostream>

namespace SE
{
  class SGameObject;
  class SScript
  {
  public:
    SScript();
    SScript(SGameObject* go);
    ~SScript();

    /*
      Initializes this script with his Luabridge configuration
      @param fliepath The path of the script to be loaded or created
    */
    void Init(const char *filepath);

    /*
      Call to the update function of the script (once per frame)
    */
    void Update();

    /*
      Reloads the script and recalls his init function
    */
    void Reload();

    /*
      Updates the GUI script text and return its value
      @return The GUI script text
    */
    char *GetScriptText();

    /*
      Saves the GUI script text
      @param text The text to be saved
    */
    void SaveScriptText(const char *text);

    /*
      Get the path of the script
      @return The path of the actual script
    */
    inline const char* GetPath() { return path.c_str(); }

  private:
    /*
      Updates the GUI script text
    */
    void LoadScriptText();

    /*
      Wrapper to the GameObject Translate function
      @param p The units to translate
    */
    void Translate(glm::vec3 p);

    /*
      Wrapper to the GameObject Rotate function
      @param r The units to rotate
    */
    void Rotate(glm::vec3 r);

    /*
      Wrapper to the GameObject Scale function
      @param s The units to scale
    */
    void Scale(glm::vec3 s);

    /*
      Wrapper to the GameObject Set Position function
      @param p The position to translate
    */
    void SetPosition(glm::vec3 p);

    /*
      Wrapper to the GameObject Set Rotation function
      @param r The rotation to be set
    */
    void SetRotation(glm::vec3 r);

    /*
      Wrapper to the GameObject Set Scale function
      @param s The scale to be set
    */
    void SetScale(glm::vec3 s);

    /*
      Wrapper to the GameObject Get Position function
      @return the position of the GameObject
    */
    glm::vec3 GetPosition();

    /*
      Wrapper to the GameObject Get Rotation function
      @return the rotation of the GameObject
    */
    glm::vec3 GetRotation();

    /*
      Wrapper to the GameObject Get Scale function
      @return the scale of the GameObject
    */
    glm::vec3 GetScale();
    
    SGameObject *gameObject;
    luabridge::lua_State* L;
    std::string path;
    char *scriptTextBuffer = nullptr;

    bool wrongCode = false;
  };
}
#endif