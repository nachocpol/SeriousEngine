/*
  SLight.cc cortizopo@esat-alumni.com
*/

#include "Engine/SLight.h"
#include "Engine/SDebug.h"
#include "Engine/SGameObject.h"
#include "Engine/SMaterialSettings.h"
#include "Engine/SFrameBuffer.h"

#include "imgui.h"

#include "gtc/matrix_transform.hpp"

namespace SE
{
  //////////////////////////

  SLight::SLight(SGameObject* goRef)
  {
    sceneCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    if (!goRef)
    {
      SDebug::GetInstance()->LogError("SLight::SLight():passed a null game object.\n");
      return;
    }
    goOwner = goRef;
  }

  //////////////////////////

  SLight::~SLight()
  {

  }

  //////////////////////////

  void SLight::Init(SLightInfo linf)
  {
    lInfo = linf;

    //Init shadow fbo
    shadowFBOMatSet = new SMaterialSettings("../data/shaders/BScreen.vs", "../data/shaders/BScreen.fs");
    shadowFBO = new SFrameBuffer(SScreenQuadConstructor(), shadowFBOMatSet);
  }

  //////////////////////////

  void SLight::SetLightDir(glm::vec3 lDir)
  {
    lInfo.lDir = glm::normalize(lDir);
  }

  glm::mat4 SLight::GetLightMatrix()
  {
    //Update and normalize light direction
    lInfo.lDir = sceneCenter - *goOwner->transform->GetPosition();
    lInfo.lDir = glm::normalize(lInfo.lDir);

    //Build the light matrix
    glm::mat4 lightProjection = glm::ortho(-lInfo.lSize, lInfo.lSize, -lInfo.lSize, lInfo.lSize, lInfo.lNear, lInfo.lFar);
    //lightProjection = glm::perspective(glm::radians(70.0f), 1.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(*goOwner->transform->GetPosition(), sceneCenter, glm::vec3(0, 1, 0));

    return lightProjection * lightView;
  }

  //////////////////////////

  void SLight::ShowGui()
  {
    ImGui::InputFloat("Near:", &lInfo.lNear);
    ImGui::InputFloat("Far:", &lInfo.lFar);
    ImGui::InputFloat("Size:", &lInfo.lSize);
    ImGui::InputFloat("CutOFF:", &lInfo.lCutOff);
    ImGui::InputFloat3("Scene Center:", &sceneCenter.x);
    ImGui::InputFloat3("Color:", &lInfo.lCol.x);

    ImGui::Text("Light Dir:%f,%f,%f",lInfo.lDir.x,lInfo.lDir.y,lInfo.lDir.z);
    ImGui::Text("Current scene center:%f,%f,%f", sceneCenter.x, sceneCenter.y, sceneCenter.z);

    ImGui::Image((ImTextureID)GetShadowTexure(), ImVec2(80.0f, 80.0f),
      ImVec2(1, 1), ImVec2(0, 0), ImColor(255, 255, 255, 255),
      ImColor(255, 255, 255, 128));
  }

  //////////////////////////

  void SLight::SetSceneCenter(glm::vec3 c)
  {
    sceneCenter = c;
  }

  //////////////////////////

  glm::vec3* SLight::GetPosition()
  {
    return goOwner->transform->GetPosition();
  }

  //////////////////////////

  int SLight::GetShadowTexure()
  {
    return shadowFBO->GetDeptStencilhTex();
  }

  //////////////////////////

  void SLight::EnableLightShadow()
  {
    shadowFBO->Enable();
  }

  //////////////////////////

  void SLight::DisableLightShadow()
  {
    shadowFBO->Disable();
  }

  //////////////////////////

  SFrameBuffer* SLight::GetLightFbo()
  {
    return shadowFBO;
  }

  //////////////////////////

  float SLight::GetLightType()
  {
    switch(lInfo.lType)
    {
    case kSun:
      return 0.0f;
      break;
    case kSpotLight:
      return 1.0f;
      break;
    }

    return -1.0f;
  }

  //////////////////////////
}