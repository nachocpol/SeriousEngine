/*
  SDisplayList.h nachocpol@gmail.com & David Parra
*/

#ifndef SDISPLAYLIST_H
#define SDISPLAYLIST_H


#include "glm.hpp"
#include "gtc/type_ptr.hpp"

#include "SRenderManager.h"
#include "SCamera.h"
#include "SMesh.h"
#include "SMaterial.h"
#include "SLight.h"


namespace SE
{
  
  ///////////////////////////////////

  class SDisplayCommand
  {
  public:
    SDisplayCommand();
    virtual ~SDisplayCommand();
    virtual void Execute();
  };

  ///////////////////////////////////
  class SUseCamera :public SDisplayCommand
  {
  public:
    SUseCamera(SCamera &cam);
    ~SUseCamera();
  private:
    
  };

  ///////////////////////////////////

  class SUseMaterial:public SDisplayCommand
  {
  public:
    SUseMaterial() = delete;
    SUseMaterial(const SUseMaterial &) = delete;

    SUseMaterial(SMaterial *m,glm::mat4 &mod);
    ~SUseMaterial();
    void Execute();

  private:
    SMaterial *mat = nullptr;
    glm::mat4 model;
  };
  
  ///////////////////////////////////

  class SDrawGeometry :public SDisplayCommand
  {
  public:
    SDrawGeometry(SMesh *me);
    ~SDrawGeometry();
    void Execute();
  private:
    SMesh *mesh = nullptr;
  };
  
  ///////////////////////////////////

  class SDisplayCache
  {
  public:
    SDisplayCache();
    ~SDisplayCache();

    glm::mat4 lastProjection;
    glm::mat4 lastView;
    glm::mat4 lastModel;

    glm::mat4 curLightMatrix;
    bool doShadowPass = false;
    //GLuint shadowMapId;
    std::vector<SLight*> curLights;

    bool useCustomMat = false;
    SMaterial* customMat = nullptr;

    std::vector<glm::vec4> clipPlanes;
    //Do not change this value
    const unsigned int maxClipPlanes = 4;

  private:
    
  };

  ///////////////////////////////////

  class SDisplayList
  {
  public:
    SDisplayList();
    ~SDisplayList();

    void Init();
    void Copy(SDisplayList *toBeCopied);
    void RunDl();
    void CleanDl();

    void AddCmd(std::unique_ptr<SDisplayCommand> cmd);

    void SetCamera(SCamera* cam);

    /*
      We set a new clip plane that will be used in the following
      draw commands.
      @param indx the index of the clip plane, note that there is a maximun
      number of clip planes
      @param plane the actual plane representation
    */
    void SetClipPlane(unsigned int indx, glm::vec4 plane);

    /*
      Sets the light matrix used for the next shadow pass
    */
    void SetCurLightMatrix(glm::mat4 lm);

    /*
      If true,the next pass will generate a shadow map,we need
      to enable a depth framebuffer and also set the current light
      matrix
    */
    void SetShadowPass(bool b);

    /*
      Sets the current shadow map id
    */
    //void SetShadowMap(GLuint id);

    /*
      Sets the current lights
    */
    void SetCurrentLights(std::vector<SLight*> &l);

    /*
      The display list will use the given custom material.
    */
    void SetUseCustomMat(bool use,SMaterial* mat);

  private:
    std::vector<std::unique_ptr<SDisplayCommand> > dl;
    
    //Draw data buffer(bind 1)
    GLuint clipBuffer = 0;
  };
}
#endif