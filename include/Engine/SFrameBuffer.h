/*
  SFrameBuffer.h cortizopo@esat-alumni.com
*/

#ifndef SFRAMEBUFFER_H
#define SFRAMEBUFFER_H

#include "SMesh.h"
#include "SMaterial.h"
#include "SRenderManager.h"

namespace SE
{
  class SFrameBuffer
  {
  public:
    /*
      Constructs this frame buffer
      @param sCons Position of the frame buffer in the screen
      @param ms The material settings of the framebuffer
    */
    SFrameBuffer(SScreenQuadConstructor sCons,SMaterialSettings* ms);
    ~SFrameBuffer();

    /*
      The following draw commands will be outputed to
      this FBO.
    */
    void Enable();
    
    /*
      Draws the content of this FBO to the screen.
      @param d If true it will be rendered to  the screen,if
      false it will be rendered to the bound FBO.
    */
    void Draw(bool d=true);

    /*
      The following draw commands wont be outputed to
      this FBO.
    */
    void Disable();

    /*
      Returns the id of the color texture.
      @return returns the id of the color texture.
    */
    int GetColorTex(){ return screenColorTex; }

    int GetDeptStencilhTex(){ return screenDepthTex; }

    GLuint screenFbo;

    SMaterial screenMat;

  private:
    SScreenQuadConstructor screenCoords;

    //Screen quad
    SMesh screenMesh;

    //Framebuffer
    GLuint screenColorTex;
    GLuint screenDepthTex;

    //Render manager
    SRenderManager* renderManRef = nullptr;
  };
}
#endif