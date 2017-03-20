/*
  SRenderManager.h nachocpol@gmail.com
*/

#include "Engine/SRenderManager.h"
#include "Engine/STaskManager.h"
#include "Engine/SScene.h"
#include "Engine/SDebug.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

namespace SE
{
  //Make sure we only have 1 instance of the class
  SRenderManager* SRenderManager::instance = NULL;

  ///////////////////////////////////////////////////////////////////

  SRenderManager *SRenderManager::GetInstance()
  {
    //std::cout << "Asking for RenderManager Instance\n";
    //Check if we are already created
    if (!instance)
    {
      instance = new SRenderManager;
    }
    return instance;
  }

  ///////////////////////////////////////////////////////////////////

  SRenderManager::SRenderManager()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SRenderManager::~SRenderManager()
  {

  }

  void SRenderManager::ShutDown()
  {
    STaskManager::GetInstance()->KillThreads();
    //delete(STaskManager::GetInstance());

    //TO-DO:make a proper CleanUp method
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
  }

  ///////////////////////////////////////////////////////////////////

  void ImGuiStyleInit()
  {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.WindowPadding = ImVec2(12, 11);
    style.FrameRounding = 5.0f;
    style.IndentSpacing = 5.0f;
  }

  bool SRenderManager::Init(int w, int h, const char* t)
  {
    if (!initialized)
    {
      targetWindow = new SWindow();
      if (!targetWindow->Init(w, h, t))
      {
        SDebug::GetInstance()->LogError("Failed to initialize window.\n");
      }

      //Set true for ensures GLEW to use more modern techniques for managing OpenGL functionality
      glewExperimental = GL_TRUE;

      //Initialize GLEW Library to use it
      if (glewInit() != GLEW_OK)
      {
        SDebug::GetInstance()->LogError("Failed to initialize GLEW.\n");
        return false;
      }

      //Set the size of the rendering window for OpenGL 
      //(location of the lower left corner(x,y), width and height)
      glViewport(0, 0, w, h);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);

      //Initialize ImGUI(for internal use)
      ImGui_ImplGlfwGL3_Init(targetWindow->GetWindow(), true);
      ImGuiStyleInit();

      //Init class variables
      clearColor = glm::vec3(1.0, 0, 0);
      windowSize = glm::vec2(w, h);

      SDebug::GetInstance()->Log("RenderManager initialized.\n");

      GLint cnt;
      glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &cnt);
      SDebug::GetInstance()->Log("Max textures units supported: %i \n",cnt);

      //Initialize task manager
      STaskManager::GetInstance()->Init();
      SDebug::GetInstance()->Log("Initialized task manager.\n");

      initialized = true;
      resMan = SResourceManager::GetInstance();
      return true;
    }
    return false;
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::MainRender()
  {
    /////////////////////////////
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, targetWindow->GetWindowSize().x, targetWindow->GetWindowSize().y);
    
    ImGui_ImplGlfwGL3_NewFrame();
    //////////////////////////////

    //We reset the draw info
    drawCalls = 0;
    drawTriangles = 0;
    //Start rendering scene from here
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::Events()
  {

    //Processes the events in the event queue and returns them, 
    //causing the window and input associated to be called
    glfwPollEvents();

    //Save initial time(ms)
    initTime = glfwGetTime();
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::Swap()
  {
    //Calculate delta time(ms)
    deltaTime = glfwGetTime() - initTime;

    ImGui::Render();
    //Swaps the front and back buffers (the rendered one and the being generated one)
    glfwSwapBuffers(targetWindow->GetWindow());
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::SetClearColor(glm::vec3 color)
  {
    clearColor = color;
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
  }

  ///////////////////////////////////////////////////////////////////

  bool SRenderManager::ShouldClose()
  {
    return glfwWindowShouldClose(targetWindow->GetWindow());
  }

  ///////////////////////////////////////////////////////////////////

  glm::vec2 SRenderManager::GetWindowSize()
  {
    return (glm::vec2)targetWindow->GetWindowSize();
  }

  ///////////////////////////////////////////////////////////////////

  GLFWwindow *SRenderManager::GetWindow()
  {
    return targetWindow->GetWindow();
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::GetAndPrintError(const char* msg)
  {
    GLenum er = glGetError();
    if (er != GL_NO_ERROR)
    {
      switch (er)
      {
      case GL_INVALID_ENUM:
        std::cout << "OpenGl error code:" << er << "(invalid enum):"<<msg << std::endl;
        break;
      case GL_INVALID_VALUE:
        std::cout << "OpenGl error code:" << er << "(invalid value):" << msg << std::endl;
        break;
      case GL_INVALID_OPERATION:
        std::cout << "OpenGl error code:" << er << "(invalid operation):" << msg << std::endl;
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cout << "OpenGl error code:" << er << "(invalid framebuffer operation):" << msg << std::endl;
        break;
      case GL_OUT_OF_MEMORY:
        std::cout << "OpenGl error code:" << er << "(out of memory):" << msg << std::endl;
        break;
      default:
        std::cout << "OpenGl error code:" << er << "(no description):" << msg << std::endl;
        break;
      }
    }
    else
    {
      std::cout << "No error at:" << msg << std::endl;
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::GetAndPrintError(const char* msg,bool logIfNoError)
  {
    static int cnt = 0;
    cnt++;

    GLenum er = glGetError();
    if (er != GL_NO_ERROR)
    {
      std::cout<<cnt;
      switch (er)
      {
      case GL_INVALID_ENUM:
        std::cout << "OpenGl error code:" << er << "(invalid enum):" << msg << std::endl;
        break;
      case GL_INVALID_VALUE:
        std::cout << "OpenGl error code:" << er << "(invalid value):" << msg << std::endl;
        break;
      case GL_INVALID_OPERATION:
        std::cout << "OpenGl error code:" << er << "(invalid operation):" << msg << std::endl;
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cout << "OpenGl error code:" << er << "(invalid framebuffer operation):" << msg << std::endl;
        break;
      case GL_OUT_OF_MEMORY:
        std::cout << "OpenGl error code:" << er << "(out of memory):" << msg << std::endl;
        break;
      default:
        std::cout << "OpenGl error code:" << er << "(no description):" << msg << std::endl;
        break;
      }
    }
    else
    {
      if (logIfNoError)
        std::cout << "No error at:" << msg << std::endl;
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::GenVertexArrayObj(int amo, GLuint &dest)
  {
    glGenVertexArrays(amo, &dest);
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::GenBuffer(int amo, GLuint &dest)
  {
    glGenBuffers(amo, &dest);
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::BindVertexArray(GLuint vao)
  {
    glBindVertexArray(vao);
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::UnbindVertexArray()
  {
    glBindVertexArray(0);
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::BindBuffer(SBufferType type, int id)
  {
    //Bind the buffer (id) to the given target

    if (type == kArray)
    {
      glBindBuffer(GL_ARRAY_BUFFER, id);
    }
    else if (type == kElement)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::UploadBufferData(SBufferType type, int size, const void * begin, SUsage usage)
  {
    //List of the GL_... uses
    int use[] = { GL_STATIC_DRAW };

    //We upload data to the current binded buffer.(BindBuffer();)
    if (type == kArray)
    {
      glBufferData(GL_ARRAY_BUFFER, size, begin, use[usage]);
    }
    else if (type = kElement)
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, begin, use[usage]);
    }
  }

  ///////////////////////////////////////////////////////////////////
  //TO-DO:now offset is 0 by default,this ins kinda unsafe (overflow)
  void SRenderManager::UploadSubData(SBufferType type, int size, const void* begin)
  {
    if (type == kArray)
    {
      glBufferSubData(GL_ARRAY_BUFFER, 0, size, begin);
    }
    else if (type == kElement)
    {
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, begin);
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::UnbindBuffer(SBufferType type)
  {
    //Unbinds the given buffer of Type type
    if (type == kArray)
    {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else if (type == kElement)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::SetVertAttribPoint(int loc, int eleSize, SDataType type, bool norm, int stride, int offset)
  {
    //This order must mach STypes order!!!
    int dataT[] = { GL_FLOAT, GL_INT };
    
    //We enable by defaul the given location
    glEnableVertexAttribArray(loc);

    //glVertexAttribPointer(destination,numberElements,data type,normalize?,stride,offset)
    //numberElements: vec2 =2,vec3=3 etc
    //stride: sizeof to find next atrrib like me
    //offest: where do I first appear in the buffer
    glVertexAttribPointer(loc, eleSize, dataT[type], norm, stride, (GLvoid*)offset);

  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::Draw(SDrawMode drawMode, int eleCnt, int vaoInd)
  {
    //We bind the VAO
    glBindVertexArray(vaoInd);
    {
      //glDrawElement(mode,elementsCnt,eleType,offset)
      //Draw a line loop
      if (drawMode == kLines)
      {
        //Remove this on release
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, eleCnt, GL_UNSIGNED_INT, 0);
      }

      //Draw the triangle mesh
      else if (drawMode == kTriangles)
      {
        //Remove this on release
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, eleCnt, GL_UNSIGNED_INT, 0);
      }
    }
    //We unbind it
    glBindVertexArray(0);

    //Update drawing information
    drawTriangles += (eleCnt / 3);
    drawCalls++;
  }

  ///////////////////////////////////////////////////////////////////
  
  double SRenderManager::GetDeltaTimeMs()
  {
    return deltaTime * 1000;
  }

  ///////////////////////////////////////////////////////////////////

  double SRenderManager::GetDeltaTimeS()
  {
    return deltaTime;
  }

  ///////////////////////////////////////////////////////////////////

  double SRenderManager::GetTime()
  {
    return glfwGetTime();
  }

  ///////////////////////////////////////////////////////////////////

  unsigned char* SRenderManager::LoadTextureFromFile(const char *p, int &x, int &y,int &n)
  {
    return stbi_load(p, &x, &y, &n, 0);
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::SetUniform(int pid,const char *name, SDataType type, const float *value, int loc)
  {
    //Check if we give the location, if not find by ourselves
    int location = loc;
    if (loc == -1)
    {
      //First find the location of the uniform
      //If the given "name" is not used we will be doing this for ever PERFORMANCE!
      location = glGetUniformLocation(pid, name);
    }

    //Set the value
    switch (type)
    {
    case SE::KFloat:
      glUniform1f(location, value[0]);
      break;
    case SE::KInt:
      glUniform1i(location, value[0]);
      break;
    case SE::kMat3:
      glUniformMatrix3fv(location, 1, false, value);
      break;
    case SE::kMat4:
      glUniformMatrix4fv(location, 1, false, value);
      break;
    case SE::kFloat4:
      glUniform4f(location, value[0], value[1], value[2], value[3]);
      break;
    case SE::kFloat3:
      glUniform3f(location, value[0], value[1], value[2]);
      break;
    case SE::kFloat2:
      glUniform2f(location, value[0], value[1]);
      break;
    }
  }

  ///////////////////////////////////////////////////////////////////

  int SRenderManager::GetUniform(int pid, const char* name)
  {
    return (int)glGetUniformLocation(pid, name);
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::SetTexture(int tUnit, STextureType type, int gpuId, int loc)
  {
    //Activate the texture unit
    glActiveTexture(GL_TEXTURE0 + tUnit);

    //Bind the texture
    switch (type)
    {
    case SE::k2DTexture:
      glBindTexture(GL_TEXTURE_2D, gpuId);
      break;
    case SE::kTextureCubeMap:
      glBindTexture(GL_TEXTURE_CUBE_MAP, gpuId);
      break;
    }
    
    //Set the uniform
    glUniform1i(loc, tUnit);
  }

  ///////////////////////////////////////////////////////////////////

  void SRenderManager::InitCubeMap(std::vector<std::string> paths)
  {
    envCubeMap = new STextureCubeMap;
    envCubeMap->Init(paths);
  }

  ///////////////////////////////////////////////////////////////////

  unsigned int SRenderManager::GetCubeMapId()
  {
    return envCubeMap->GetId();
  }

  ///////////////////////////////////////////////////////////////////
}