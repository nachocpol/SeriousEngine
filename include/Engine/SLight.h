/*
  SLight.h cortizopo@esat-alumni.com
*/

#ifndef SLIGHT_H
#define SLIGHT_H

#include "SComponent.h"

namespace SE
{
  class SFrameBuffer;
  class SMaterialSettings;
  class SGameObject;

  class SLight :public SComponent
  {
  public:

    SLight();

    /*
      Constructor
      @param goRef The owner game object
    */
    SLight(SGameObject* goRef);

    /*
      Default destructor
    */
    ~SLight();

    /*
      Sets the configuration of this light
    */
    void Init(SLightInfo linf);

    /*
      Updates this component
    */
    void Update();

    void SetLightDir(glm::vec3 lDir);

    /*
      Returns the light matrix
      @return Returns the light matrix
    */
    glm::mat4 GetLightMatrix();

    /*
      Returns the light info.
      @return Returns the light info.
    */
    SLightInfo GetLightInfo(){ return lInfo; }
      
    /*
      Shows the gui of the light component.
    */
    void ShowGui();

    /*
      Sets the current center position of the scene.
    */
    void SetSceneCenter(glm::vec3 c);

    /*
      Returns the position of this light.
      @return Returns the position of this light.
    */
    glm::vec3* GetPosition();

    /*
      Returns the gpu id of the shadow texture.
      @return Returns the gpu id of the shadow texture.
    */
    int GetShadowTexure();

    void EnableLightShadow();

    void DisableLightShadow();

    SFrameBuffer* GetLightFbo();

    /*
      Returns the type of this light.
      @return Returns the type of this light.
    */
    float GetLightType();

  private:
    SGameObject* goOwner = nullptr;
    SLightInfo lInfo;
    glm::vec3 sceneCenter;

    //Shadow buffer
    SFrameBuffer *shadowFBO = nullptr;
    SMaterialSettings* shadowFBOMatSet = nullptr;

  };
}
#endif