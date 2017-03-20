/*
  SMaterialSettings.cc cortizopo@esat-alumni.com
*/

#include "Engine/SMaterialSettings.h"
#include "Engine/SRenderManager.h"
#include "Engine/SResourceManager.h"

namespace SE
{
  ///////////////////////////////////////////////////////////////////

  SMaterialSettings::SMaterialSettings()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SMaterialSettings::SMaterialSettings(const char* vs, const char* fs)
  {
    vsp = vs;
    fsp = fs;
  }

  ///////////////////////////////////////////////////////////////////

  SMaterialSettings::~SMaterialSettings()
  {
    
  }


  void SMaterialSettings::ShowGui()
  {

  }

  ///////////////////////////////////////////////////////////////////

  void SMaterialSettings::GetLocations(int pid)
  {

  }

  ///////////////////////////////////////////////////////////////////

  void SMaterialSettings::SetLocations(int pid)
  {

  }

  ///////////////////////////////////////////////////////////////////

  SPhysicalMatSet::SPhysicalMatSet(const char* vs, const char* fs, const char* dPth, const char* glosPth, const char* specPth, const char* normPth)
  {
    SResourceManager* rMan = SResourceManager::GetInstance();

    vsp       = vs;
    fsp       = fs;
    diffPath  = dPth;
    glossPath = glosPth;
    specPath  = specPth;
    normPath  = normPth;

    //Add the textures to theresource manager
    rMan->AddTexture(diffPath);
    rMan->AddTexture(normPath);
    rMan->AddTexture(glossPath);
    rMan->AddTexture(specPath);
    rMan->AddTexture(preIntPath);
  }

  ///////////////////////////////////////////////////////////////////

  SPhysicalMatSet::~SPhysicalMatSet()
  {
    SResourceManager* resMan = SResourceManager::GetInstance();
  }

  ///////////////////////////////////////////////////////////////////

  void SPhysicalMatSet::GetLocations(int pid)
  {
    SRenderManager* rendMn = SRenderManager::GetInstance();

    glossMapLocation    = rendMn->GetUniform(pid, "uGloss");
    normalMapLocation   = rendMn->GetUniform(pid, "uNormal");
    diffuseLocation     = rendMn->GetUniform(pid, "uAlbedo");
    specularMapLocation = rendMn->GetUniform(pid, "uSpecular");
    preIntMapLocation   = rendMn->GetUniform(pid, "uIntegratedFG");

    cameraPosLocation   = rendMn->GetUniform(pid, "uCamPos");
    cubeMapLocation     = rendMn->GetUniform(pid, "uEnvCubeMap");
  }

  ///////////////////////////////////////////////////////////////////

  void SPhysicalMatSet::SetLocations(int pid)
  {
    SRenderManager* rendMn = SRenderManager::GetInstance();
    SResourceManager* resMan = SResourceManager::GetInstance();

    //Set uniforms
    SRenderManager::GetInstance()->SetUniform(pid, "uCamPos", kFloat3, &refCamPos->x, cameraPosLocation);

    //Text unit 0
    rendMn->SetTexture(0, kTextureCubeMap, rendMn->GetCubeMapId(), cubeMapLocation);
    /*
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap.GetId());
    glUniform1i(cubeMapLocation, 0);
    */

    //Text unit 1
    rendMn->SetTexture(1, k2DTexture, resMan->GetTexture(diffPath)->GetId(),diffuseLocation);
    /*
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, SE::SResourceManager::GetInstance()->GetTexture(diffPath)->GetId());
    glUniform1i(diffuseLocation, 1);
    */

    //Text unit 2
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, SE::SResourceManager::GetInstance()->GetTexture(normPath)->GetId());
    glUniform1i(normalMapLocation , 2);

    //Text unit 3
    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, SE::SResourceManager::GetInstance()->GetTexture(glossPath)->GetId());
    glUniform1i(glossMapLocation, 3);

    //Text unit 4
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, SE::SResourceManager::GetInstance()->GetTexture(preIntPath)->GetId());
    glUniform1i(preIntMapLocation, 4);

    //Text unit 5
    glActiveTexture(GL_TEXTURE0 + 5);
    glBindTexture(GL_TEXTURE_2D, SE::SResourceManager::GetInstance()->GetTexture(specPath)->GetId());
    glUniform1i(specularMapLocation, 5);
  }

  ///////////////////////////////////////////////////////////////////

  void SPhysicalMatSet::ShowGui()
  {
    float f[4] = { 1.0, 0.0, 0.0, 1.0 };
    ImGui::InputFloat4("Test", f, 1);

    ImGui::Image((ImTextureID)SE::SResourceManager::GetInstance()->GetTexture(diffPath)->GetId(), ImVec2(80.0f, 80.0f),
      ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255),
      ImColor(255, 255, 255, 128));

  }

  ///////////////////////////////////////////////////////////////////

  SBaseScreenMaterialSet::SBaseScreenMaterialSet()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SBaseScreenMaterialSet::SBaseScreenMaterialSet(const char* vs, const char* fs)
  {
    vsp = vs;
    fsp = fs;
  }

  ///////////////////////////////////////////////////////////////////

  SBaseScreenMaterialSet::~SBaseScreenMaterialSet()
  {

  }

  ///////////////////////////////////////////////////////////////////

  void SBaseScreenMaterialSet::GetLocations(int pid)
  {
	  depthTextureLoc = glGetUniformLocation(pid, "uDepthTexture");
    screenTextureLoc = glGetUniformLocation(pid, "uScreenTexture");
    screenResLoc = glGetUniformLocation(pid, "uScreenResolution");
  }

  ///////////////////////////////////////////////////////////////////

  void SBaseScreenMaterialSet::SetLocations(int pid)
  {
    screenRes = SRenderManager::GetInstance()->GetWindowSize();
    SRenderManager::GetInstance()->SetUniform(pid, "uScreenResolution",kFloat2, glm::value_ptr(screenRes), screenResLoc);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, screenColorTexture);
    glUniform1i(screenTextureLoc, 0);

	  glActiveTexture(GL_TEXTURE0 + 1);
	  glBindTexture(GL_TEXTURE_2D, depthTexture);
	  glUniform1i(depthTextureLoc, 1);
  }

  ///////////////////////////////////////////////////////////////////

  void SBaseScreenMaterialSet::ShowGui()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SShadowMaterialSet::SShadowMaterialSet(const char* vs, const char* fs)
  {
    vsp = vs;
    fsp = fs;
  }

  SShadowMaterialSet::~SShadowMaterialSet()
  {

  }

  ///////////////////////////////////////////////////////////////////

  void SShadowMaterialSet::GetLocations(int pid)
  {
    lightSpaceMatrix = glGetUniformLocation(pid, "uLightSpaceMatrix");
  }

  ///////////////////////////////////////////////////////////////////

  void SShadowMaterialSet::SetLocations(int pid, glm::mat4 lm)
  {
    SRenderManager::GetInstance()->SetUniform(pid, "uLightSpaceMatrix", kMat4, glm::value_ptr(lm), lightSpaceMatrix);
  }

  ///////////////////////////////////////////////////////////////////

  SBasicTextureMatSet::SBasicTextureMatSet(const char* vs, const char* fs, const char* texture)
  {
	  SResourceManager* rMan = SResourceManager::GetInstance();

	  vsp = vs;
	  fsp = fs;

	  //Add the textures to theresource manager
	  rMan->AddTexture(texture);
    colorTexture = rMan->GetTexture(texture)->GetId();
    lightTexture = rMan->GetTexture(texture)->GetId();
  }

  ///////////////////////////////////////////////////////////////////

  SBasicTextureMatSet::~SBasicTextureMatSet()
  {
	  SResourceManager* resMan = SResourceManager::GetInstance();
  }

  ///////////////////////////////////////////////////////////////////

  void SBasicTextureMatSet::GetLocations(int pid)
  {
	  SRenderManager* rendMn = SRenderManager::GetInstance();

	  cameraPosLocation = rendMn->GetUniform(pid, "uCamPos");
    sunPosLocation = rendMn->GetUniform(pid, "uSunPos");
	  colorLocation = rendMn->GetUniform(pid, "uColor");

	  textureLocation = rendMn->GetUniform(pid, "uTexture");
    lightTextureLocation = rendMn->GetUniform(pid, "uLightTexture");
  }

  ///////////////////////////////////////////////////////////////////

  void SBasicTextureMatSet::SetLocations(int pid)
  {
	  SRenderManager* rendMn = SRenderManager::GetInstance();
	  SResourceManager* resMan = SResourceManager::GetInstance();

	  //Set uniforms
    SRenderManager::GetInstance()->SetUniform(pid, "uColor", kFloat3, &color.x, colorLocation);

    if (refSunPos != nullptr)
      SRenderManager::GetInstance()->SetUniform(pid, "uSunPos", kFloat3, &refSunPos->x, sunPosLocation);

    if (refCamPos != nullptr)
      SRenderManager::GetInstance()->SetUniform(pid, "uCamPos", kFloat3, &refCamPos->x, cameraPosLocation);

	  //Text unit 0
    rendMn->SetTexture(0, k2DTexture, colorTexture, textureLocation);

    rendMn->SetTexture(1, k2DTexture, lightTexture, lightTextureLocation);
  }
}