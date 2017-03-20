/*
  SRenderManager.h nachocpol@gmail.com
*/

#ifndef SRENDERMANAGER_H
#define SRENDERMANAGER_H


#include <iostream>
#include <vector>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "STypes.h"
#include "SDisplayList.h"
#include "SWindow.h"
#include "SResourceManager.h"
#include "STextureCubeMap.h"

namespace SE
{
  class SRenderManager
  {
  public:
    static SRenderManager *GetInstance();
    SRenderManager();
    ~SRenderManager();

    /**
      Initializes the rendering window and the render context.
      @param w the rendering width
      @param h the rendering height
      @param t the title
      @return True if sucessful,false if not
    */
    bool Init(int w, int h,const char* t);

    /**
      This function should be called before drawing a new frame.
    */
    void MainRender();

    /**
      Swaps the front and back buffers (the rendered one and the being generated one)
      This function should be called after rendering all the frame, any drawing commands
      after this call WILL BE IGNORED.
    */
    void Swap();

    /**
      Process window events.
    */
    void Events();

    /*
      Sets the current clear color.
      @param color The clear color
    */
    void SetClearColor(glm::vec3 color);

    /*
      Returns the current clear color.
      @return Returns the current clear color.
    */
    glm::vec3* GetClearColor(){ return &clearColor; }

    /**
      Returns true if the window is going to close.
      @return Returns true if the window is going to close.
    */
    bool ShouldClose();

    /**
      Cleans the engine.
    */
    void ShutDown();

    /**
      Returns the current window size in pixels.
      @return Returns the current window size in pixels.
    */
    glm::vec2 GetWindowSize();

    /**
      Returns the current window.
      @return Returns the current window.
    */
    GLFWwindow *GetWindow();

    /**
      Checks if there was an error in the last GPU call.
      @param msg Message that will be shown with the error.
    */
    void GetAndPrintError(const char* msg);

    /**
      Checks if there was an error in the last GPU call.
      @param msg Message that will be shown with the error.
      @param logIfNoError Should we print the log if there are no errors?
    */
    void GetAndPrintError(const char* msg,bool logIfNoError);

    /**
      Generates a VAO.
      @param amo Ammount of vaos to generate
      @param dest The indices to be initialized
    */
    void GenVertexArrayObj(int amo, GLuint &dest);

    /**
      Generates GPU buffers.
      @param amo Ammount of vaos to generate
      @param dest The indices to be initialized
    */
    void GenBuffer(int amo, GLuint &dest);

    /**
      Binds to a VAO
      @param vao The id of the VAO that we want to bind
    */
    void BindVertexArray(GLuint vao);

    /**
      Unbinds the current VAO
    */
    void UnbindVertexArray();

    /**
      Binds to a given buffer.
      @param type The type of the buffer to be binded
      @param id The identifier of the buffer
    */
    void BindBuffer(SBufferType type, int id);

    /**
      Upload data to a buffer in the GPU (should bind before doing this).
      @param type The type of the buffer to be binded
      @param size The size that we will uplaod to the GPU
      @param begin Where does the data starts
      @param usage How this buffer will be used
    */
    void UploadBufferData(SBufferType type, int size, const void* begin, SUsage usage);

    /**
      Uploads (and overrides) data to the current bind buffer.
      @param type The type of the buffer to be binded
      @param size The size that we will uplaod to the GPU
      @param begin Where does the data starts
    */
    void UploadSubData(SBufferType type,int size, const void* begin);

    /**
      Unbinds the current bound buffer.
      @param type The buffer target to unbind
    */
    void UnbindBuffer(SBufferType type);

    /**
      Sets a vertex attribute.
      @param loc The location of the attribute in the shader.
      @param eleSize How many elements compose this attribute? (Vec3:3)
      @param type The type of the attribute
      @param norm Should we normalize this attribute?
      @param stride Space between sucesive attributes of the same type
      @param offset Space to find the first item of this type
    */
    void SetVertAttribPoint(int loc, int eleSize, SDataType type, bool norm, int stride, int offset);

    /**
      Draws some geometry.
      @param drawMode The drawing mode used with the given elements
      @param eleCnt Number of elements that will be painted
      @param vaoInd The index of the vao for this rendered object 
    */
    void Draw(SDrawMode drawMode, int eleCnt, int vaoInd);

    /**
      Returns the delta time in ms
      @return Returns the delta time in ms
    */
    double GetDeltaTimeMs();

    /**
      Returns the time in seconds.
      @return Returns the time in seconds.
    */
    double GetDeltaTimeS();

    /*
      Returns the time since initialization.(seconds)
      @return Returns the time since initialization.(seconds)
    */
    double GetTime();

    /**
      Loads the texture from the hard drive
      @param p The path
      @param x Int to save the width
      @param y Int to save the height
      @param n Number of bytes per pixel.
      @return The loaded image
    */
    unsigned char* LoadTextureFromFile(const char *p, int &x, int &y,int &n);

    /*
      Sets the value of the given uniform.
      @param pid The id of the program.
      @param name The name of the uniform.
      @param type The type of the uniform
      @param value The value of the uniform.
      @param loc The location of that uniform.
    */
    void SetUniform(int pid,const char* name,SDataType type,const float* value,int loc=-1);

    /*
      Gets the GPU value of the given uniform.
      @param pid The program id to check.
      @param name The name of the uniform
      @return Returns the id of the uniform (-1 if it does not exist)
    */
    int GetUniform(int pid,const char* name);

    /*
      Sets a texture.
      @param tUnit The unit to bind the texture.
      @param type The type of the texture.
      @param gpuId The id of the texture object.
      @param loc The location of the uniform.
    */
    void SetTexture(int tUnit,STextureType type,int gpuId,int loc);

    inline unsigned int* GetDrawCalls(){ return &drawCalls; }

    inline unsigned int* GetDrawTriangles(){ return &drawTriangles; }

    /*
      Initializes the global cube map of the scene.
      @params paths The paths of the cube maps 
      textures.
    */
    void InitCubeMap(std::vector <std::string> paths);

    /*
      Returns the global cube map gpu id.
      @return Returns the global cube map gpu id.
    */
    unsigned int GetCubeMapId();

  private:

    SRenderManager(SRenderManager const&){};
    SRenderManager& operator=   (SRenderManager const&){};
    static SRenderManager* instance;

    SResourceManager* resMan = nullptr;

    glm::vec3 clearColor;
    glm::vec2 windowSize;
    SWindow *targetWindow;

    bool initialized = false;

    double deltaTime = 1.0f;
    double initTime;

    unsigned int drawCalls = 0;
    unsigned int drawTriangles = 0;

    //The current cube map for the scene
    STextureCubeMap* envCubeMap = nullptr;
  };
}
#endif