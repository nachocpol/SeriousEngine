/*
  SBaseScreenMaterial.cc cortizopo@esat-alumni.com
*/

#include "Engine/SBaseScreenMaterial.h"

namespace SE
{
  ///////////////////////////////////////////////////////////////////

  SBaseScreenMaterial::SBaseScreenMaterial()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SBaseScreenMaterial::~SBaseScreenMaterial()
  {

  }

  ///////////////////////////////////////////////////////////////////

  void SBaseScreenMaterial::Init(std::string vsPath, std::string fsPath)
  {
    SMaterial::Init(&SMaterialSettings(vsPath.c_str(),fsPath.c_str()));
    ////////////////////////////////

    //Get locations
    screenTextureLoc = glGetUniformLocation(programId,"uScreenTexture"); 
  }

  ///////////////////////////////////////////////////////////////////

  void SBaseScreenMaterial::Use(int texId)
  {
    //Use this program id
    glUseProgram(programId);

    //Bind the screen texture
    //Text unit 0
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D,texId );
    glUniform1i(screenTextureLoc, 1);
  }

  ///////////////////////////////////////////////////////////////////
}