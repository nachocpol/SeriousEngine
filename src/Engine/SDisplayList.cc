/*
  SDisplayList.cc nachocpol@gmail.com & David Parra
*/

#include "Engine/SDisplayList.h"


namespace SE
{
  static SDisplayCache cache;

  static SMaterial *shadowMat = nullptr;
  static SShadowMaterialSet *shadowMatSet = nullptr;

  ///////////////////SDisplayCommand////////////////
  SDisplayCommand::SDisplayCommand()
  {

  }

  ///////////////////

  SDisplayCommand::~SDisplayCommand()
  {

  }

  ///////////////////

  void SDisplayCommand::Execute()
  {

  }

  ////////////////////SUseCamera///////////////////////

  SUseCamera::SUseCamera(SCamera &cam)
  {
    //We use default coppy constructor because we make a 
    //member copy
    //If we need a deep copy (copy a pointer content not just the direction)
    //we need to make our copy constructor
    //http://www.cplusplus.com/articles/y8hv0pDG/

    cache.lastProjection = glm::mat4(*cam.GetProjection());
    cache.lastView = glm::mat4(*cam.GetView());
  }

  ///////////////////

  SUseCamera::~SUseCamera()
  {

  }

  //////////////SUseMaterial///////////////////////

  SUseMaterial::SUseMaterial(SMaterial *m,glm::mat4 &mod)
  {
    //this is not a copy is a reference
    mat = m;
    model = glm::mat4(mod);
  }

  ///////////////////

  SUseMaterial::~SUseMaterial()
  {

  }
  
  ///////////////////

  void SUseMaterial::Execute()
  {
    //If this is a shadow pass,use the shadow map generator
    if (cache.doShadowPass)
    {
      if (shadowMat != nullptr)
      {
        shadowMat->Use(cache.curLightMatrix, model);
      }
      return;
    }
    //Use a custom material
    if(cache.useCustomMat)
    {
      if(cache.customMat)
      {
        cache.customMat->Use(model,cache.curLights);
      }
    }
    //Use the given material
    if (mat != nullptr)
    {
      mat->Use(model,cache.curLights);
    }
  }

  /////////////SDrawGeometry///////////////////////

  SDrawGeometry::SDrawGeometry(SMesh *me)
  {
    mesh = me;
  }

  ///////////////////

  SDrawGeometry::~SDrawGeometry()
  {

  }

  ///////////////////

  void SDrawGeometry::Execute()
  {
    if (mesh != nullptr)
    {
      mesh->MeshRender();
    }
  }

  /////////////////SDisplayCache//////////////////

  SDisplayCache::SDisplayCache()
  {
    //Reserve memory for the clipping planes
    clipPlanes.resize(maxClipPlanes);

    //Enable all the planes
    for (unsigned int i = 0; i < cache.maxClipPlanes; i++)
    {
      glEnable(GL_CLIP_DISTANCE0 + i);
    }
  }

  ///////////////////

  SDisplayCache::~SDisplayCache()
  {

  }

  /////////////////SDisplayList///////////////////

  SDisplayList::SDisplayList()
  {
    //We bind the clip uniform buffer (1)
    glGenBuffers(1, &clipBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, clipBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, clipBuffer, 0,  sizeof(glm::vec4));
  }

  ///////////////////

  SDisplayList::~SDisplayList()
  {
    glDeleteBuffers(1, &clipBuffer);
  }
  
  ///////////////////

  void SDisplayList::Init()
  {
    if (shadowMat == nullptr)
    {
      shadowMatSet = new SShadowMaterialSet("../data/shaders/ShadowShader.vs", "../data/shaders/ShadowShader.fs");
    }
    if (shadowMat == nullptr)
    {
      shadowMat = new SMaterial();
      shadowMat->Init(shadowMatSet);
    }
  }

  ///////////////////

  void SDisplayList::Copy(SDisplayList *toBeCopied)
  {
    CleanDl();
    for (unsigned int i = 0; i < toBeCopied->dl.size(); i++)
    {
      dl.push_back(std::move(toBeCopied->dl[i]));
    }
  }

  ///////////////////

  void SDisplayList::RunDl()
  {
    //Execute the commands of this display list
    for (unsigned int i = 0; i < dl.size(); i++)
    {
      dl[i]->Execute();
    }
  }

  ///////////////////

  void SDisplayList::CleanDl()
  {
    dl.clear();
  }

  ///////////////////

  void SDisplayList::AddCmd(std::unique_ptr<SDisplayCommand> cmd)
  {
    dl.push_back(std::move(cmd));
  }

  ///////////////////

  void SDisplayList::SetCamera(SCamera* cam)
  {
    cache.lastProjection = *cam->GetProjection();
    cache.lastView = *cam->GetView();
  }

  ///////////////////

  void SDisplayList::SetClipPlane(unsigned int indx, glm::vec4 plane)
  {
    if (indx > cache.maxClipPlanes)
    {
      //TO-DO:add a log
      return;
    }
    cache.clipPlanes[indx] = plane;

    //Upload the clipping plane to the cache
    glBindBuffer(GL_UNIFORM_BUFFER, clipBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, indx*sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(cache.clipPlanes[indx]));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
  ///////////////////

  void SDisplayList::SetCurLightMatrix(glm::mat4 lm)
  {
    cache.curLightMatrix = lm;
  }

  ///////////////////

  void SDisplayList::SetShadowPass(bool b)
  {
    cache.doShadowPass = b;
  }

  ///////////////////

  /*
  void SDisplayList::SetShadowMap(GLuint id)
  {
    cache.shadowMapId = id;
  }
  */

  ///////////////////

  void SDisplayList::SetCurrentLights(std::vector<SLight*> &l)
  {
    cache.curLights = l;
  }

  ///////////////////

  void SDisplayList:: SetUseCustomMat(bool use,SMaterial* mat)
  {
    cache.useCustomMat = use;
    cache.customMat = mat;
  }

  ///////////////////
}