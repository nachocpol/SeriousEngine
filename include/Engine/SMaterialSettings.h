/*
  SMaterialSettings.h cortizopo@esat-alumni.com
  Here you can define your own material settings for 
  custom materials.
*/

#ifndef SMATERIALSETTINGS_H
#define SMATERIALSETTINGS_H

#include <string>

#include "glm.hpp"

#include "STextureCubeMap.h"

namespace SE
{
  //Base class
  class SMaterialSettings
  {
  public:
    SMaterialSettings();
    SMaterialSettings(const char* vs, const char* fs);
    virtual ~SMaterialSettings();
    virtual void GetLocations(int pid);
    virtual void SetLocations(int pid);
    virtual void ShowGui();

    bool setDefaultAttributes = true;
    const char* vsp;
    const char* fsp;
  };

  //Material settings for the physical shader
  class SPhysicalMatSet : public SMaterialSettings
  {
  public:
    SPhysicalMatSet(const char* vs, const char* fs,
                    const char* dPth,const char* glosPth,const char* specPth,const char* normPth);
    ~SPhysicalMatSet();
    void GetLocations(int pid);
    void SetLocations(int pid);
    void ShowGui();

    glm::vec3* refCamPos;

    std::string diffPath;
    std::string normPath;
    std::string glossPath;
    std::string specPath;

  private:
    STextureCubeMap envCubeMap;

    int cameraPosLocation;
    int colorLocation;
    int cubeMapLocation;
    int diffuseLocation;
    int normalMapLocation;
    int glossMapLocation;
    int specularMapLocation;
    int preIntMapLocation;

    const std::string preIntPath = "../data/img/pbr/integrateBrdf.png";
  };

  //Material settings for the base fbo shader
  class SBaseScreenMaterialSet :public SMaterialSettings
  {
  public:
    SBaseScreenMaterialSet();
    SBaseScreenMaterialSet(const char* vs, const char* fs);
    ~SBaseScreenMaterialSet();
    void GetLocations(int pid);
    void SetLocations(int pid);
    void ShowGui();

    int screenColorTexture;
  	int depthTexture;

  private:
	  int depthTextureLoc;
    int screenTextureLoc;
    int screenResLoc;
    glm::vec2 screenRes;
  };

  //Material settings for the shadow depth
  class SShadowMaterialSet :public SMaterialSettings
  {
  public:
    SShadowMaterialSet(const char* vs, const char* fs);
    ~SShadowMaterialSet();
    void GetLocations(int pid);
    void SetLocations(int pid, glm::mat4 lm);

  private:
    int lightSpaceMatrix;
  };

  class SBasicTextureMatSet : public SMaterialSettings
  {
  public:
    SBasicTextureMatSet(const char* vs, const char* fs, const char* texture);
    ~SBasicTextureMatSet();
	  void GetLocations(int pid);
	  void SetLocations(int pid);

    glm::vec3 *refSunPos = nullptr;
	  glm::vec3* refCamPos = nullptr;
	  glm::vec3 color;

    int colorTexture;
    int lightTexture;

  private:
    int sunPosLocation;
	  int cameraPosLocation;
	  int colorLocation;
	  int textureLocation;
    int lightTextureLocation;
  };
}

#endif