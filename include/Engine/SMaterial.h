/*
  SMaterial.h nachocpol@gmail.com
*/
#ifndef SMATERIAL_H
#define SMATERIAL_H

#include <iostream>
#include <memory>
#include <string>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "SCamera.h"
#include "STypes.h"
#include "SComponent.h"
#include "STexture.h"
#include "STextureCubeMap.h"
#include "SMaterialSettings.h"


namespace SE
{
  class SRenderManager;
  class SLight;

  class SMaterial : public SComponent
  {
  public:
    SMaterial();
    ~SMaterial();

    /**
      Initializes this material object.
      @param matSet The material settins applied to this
      material(you should keep a reference of them)
    */
    void Init(SMaterialSettings* matSet);

    /**
      Reloads the material shaders
    */
    void Reload();

    /*
      Just enable material and material settings
    */
    virtual void Use();

    /*
      Used in the normal pass
      @param m Model matrix
      @param l The lights in the scene
    */
    virtual void Use(glm::mat4 &m,std::vector<SLight*> &l);

    /*
      Used for the shadow pass
      @param lm The light matrix
      @param m The model
    */
    virtual void Use(glm::mat4 lm, glm::mat4 &m);

    /*
      Returns the internal id of this material.
      @return Returns the internal id of this material.
    */
    inline int GetId(){ return programId; }

    /*
      Creates a new mat settings for this material,it will be
      hold INSIDE this material.
      TO-DO:check if initialized and also delete it!!!
    */
    virtual void CreateMaterialSettings(SCamera* sceneCam);


    SMaterialSettings* matSettings;

  protected:
    int programId;

  private:
    /**
      Loads the shader from disc
      @param path The path to the shader 
      @param out Where should we save the loaded data
    */
    void LoadFromFile(const char* path, char * &out);

    /**
      Initializes this material shaders
      @param vsPath The path to the vertex shader 
      @param fsPath The path to the fragment shader
      @param reload If true it will relaod the shader.
    */
    void InitShaders(const char* vsPath,const char* fsPath,bool reload = false);

    void ShowDebug();
    
    //Reference to the render manager
    SRenderManager* rendMang;

    //Id of the vertex and fragment shaders
    int vertShaderId;
    int fragShaderId;

    //Default material locations
    int modelLocation;
    int shadowMapLocation;
  };

  ///////////////////////////////
}
#endif