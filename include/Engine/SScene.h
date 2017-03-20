/*
  SScene.h nachocpol@gmail.com & David Parra
*/

#ifndef SSCENE_H
#define SSCENE_H

#include <vector>

#include "STaskManager.h"
#include "SDisplayList.h"
#include "SGameObject.h"
#include "SFrameBuffer.h"
#include "SCamera.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace SE
{
  class SScene
  {
  public:
    SScene();
    ~SScene();
    
    /*
      You should call this method at least once!
    */
    void Init();

    /**
      Sets the camera that will render this scene.
      @param c The camera
    */
    void SetCamera(SCamera &c);

    void MoveCamera(glm::vec3 p);

    /**
      Adds a game object to the scene.
      @param go The game object
    */
    int AddGameObject();

    /*
      Updates the game objects in this scene and generates
      a display list.
      You can override it to make more compless scene updates.
    */
    virtual void Update();

    /**
      Renders the scene.You should override it to make your 
      own renderer.
    */
    virtual void Render();

    void RenderToTexture(SBasicTextureMatSet *mat, glm::vec3 sceneToRender);

    /**
      Reloads all the materials of the scene.
    */
    void ReloadShaders();

    //List of current game objects
    std::vector<SGameObject*> goList;

    //List of current active lights
    std::vector<SLight*> lights;

    int cameraGO;

  protected:

    /*
      Renders the Graphic User Interface
    */
    virtual void RenderGUI();

    /*
      Adds a GameObject to the scene
      @param name the name of the GameObject
    */
    int AddGameObject(const char *name);

    /*
      Deletes a GameObject of the scene
      @param id The id of the GameObject to be deleted
    */
    void DeleteGameObject(const int id);

    //Each frame it will be filled with rendering commands
    SE::STaskHandler task;
    SDisplayList* displayList = nullptr;

    //The current camera used
    SCamera *mainCamera = nullptr;

    //Some references
    SRenderManager* renderManRef = nullptr;

    //Selected GO(GUI)
    int curSelectedGo = -1;

    //Draw data buffer
    GLuint dDataBuffer = -1;

    //Test frame buffer
    SBaseScreenMaterialSet* finalFboMs = nullptr;
    SFrameBuffer* finalFbo = nullptr;

    SBaseScreenMaterialSet* finalFboMs2 = nullptr;
    SFrameBuffer* finalFbo2 = nullptr;

    //Shadow buffer
    //SFrameBuffer *shadowFBO = nullptr;
    //SMaterialSettings* shadowFBOMatSet = nullptr;

    //Show/Hide GUI as default
    bool hideGUI = false;

    //This is temporal
    SE::SPhysicalMatSet *defaultMatSet;
  };
}
#endif