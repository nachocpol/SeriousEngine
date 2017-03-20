/*
  SFrameBuffer.cc cortizopo@esat-alumni.com
*/

#include "Engine/SFrameBuffer.h"

namespace SE
{
  ////////////////////////////////////////////////

  SFrameBuffer::SFrameBuffer(SScreenQuadConstructor sCons,SMaterialSettings* ms)
  {
    //Save coords
    screenCoords = sCons;

    //Save a reference to the render manager
    renderManRef = SRenderManager::GetInstance();

    //Initialize the screen framebuffer
    glGenFramebuffers(1, &screenFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFbo);

    //Initialize color texture
    glGenTextures(1, &screenColorTex);
    glBindTexture(GL_TEXTURE_2D, screenColorTex);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB,
                  renderManRef->GetWindowSize().x, renderManRef->GetWindowSize().y, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Attach the color texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenColorTex, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Initialize depth texture
    glGenTextures(1, &screenDepthTex);
    glBindTexture(GL_TEXTURE_2D, screenDepthTex);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,renderManRef->GetWindowSize().x, renderManRef->GetWindowSize().y, 0,GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,
      renderManRef->GetWindowSize().x, renderManRef->GetWindowSize().y, 0,
      GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //Attach the depth texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, screenDepthTex, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, screenDepthTex, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
   
    //Check the status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      std::cout << "SScene::SScene():The framebuffer status is invalid.\n";
    }

    //Unbind it
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Data for the screen quad
    SBinaryModelData bModelDat;
    
    bModelDat.vertices = {  screenCoords.bl.x, screenCoords.bl.y, screenCoords.bl.z,
                            screenCoords.br.x, screenCoords.br.y, screenCoords.br.z,
                            screenCoords.tr.x, screenCoords.tr.y, screenCoords.tr.z,
                            screenCoords.tl.x, screenCoords.tl.y, screenCoords.tl.z,

                            0.0f, 1.0f,    //Bottom-left (uv)
                            1.0f, 1.0f,    //Bottom-right(uv)
                            1.0f, 0.0f,    //Top-right(uv)
                            0.0f, 0.0f };  //Top-left(uv) 
                            

    bModelDat.vertexSize = 12;
    bModelDat.UVSize = 8;     
    bModelDat.indices = { 0, 1, 2,
                          0, 2, 3 };
    bModelDat.indicesSize = 6;

    //Make mesh constructor
    SMeshConstructorProcedural quadCons(kTriangles, bModelDat);

    screenMesh.Init(quadCons);
    screenMat.Init(ms);
  }

  ////////////////////////////////////////////////

  SFrameBuffer::~SFrameBuffer()
  {
    glDeleteTextures(1, &screenColorTex);
    glDeleteTextures(1, &screenDepthTex);
    glDeleteFramebuffers(1, &screenFbo);
  }

  ////////////////////////////////////////////////

  void SFrameBuffer::Enable()
  {
    //Bind the scene frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, screenFbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
  }

  ////////////////////////////////////////////////

  void SFrameBuffer::Draw(bool d)
  {
    if (d)
    {
      Disable();
    }

    //Paint a quad with the texture
    screenMat.Use();
    screenMesh.MeshRender();
  }

  ////////////////////////////////////////////////

  void SFrameBuffer::Disable()
  {
    //Sets the current buffer to the screen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  ////////////////////////////////////////////////
}