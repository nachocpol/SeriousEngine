/*
  SWindow.cc nachocpol@gmail.com
*/

//TODO:add global var/func to ask the window h/w


#include "Engine/Swindow.h"
namespace SE
{
  ///////////////////////////////////////////////////////////////////

  SWindow::SWindow()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SWindow::~SWindow()
  {
    Release();
  }

  ///////////////////////////////////////////////////////////////////

  bool SWindow::Init(int w, int h, const char* title)
  {
    //Initialize GLFW Library to use it
    glfwInit();

    //Set hints(pistas) for the next call of "glfwCreateWindow"
    //Set OpenGL version 3.X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //Set OpenGL version X.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Set OpenGL Profile to Core Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Set window to be  resizable
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(mode->width, mode->height, title, nullptr, nullptr);
    if (window == nullptr)
    {
      Release();
      return false;
    }
    //Set the window to the current OpenGL context
    glfwMakeContextCurrent(window);

    return true;
  }

  ///////////////////////////////////////////////////////////////////

  void SWindow::Release()
  {
    //Destroy this window context
    glfwDestroyWindow(window);
  }

  ///////////////////////////////////////////////////////////////////

  GLFWwindow* SWindow::GetWindow()
  {
    return window;
  }

  ///////////////////////////////////////////////////////////////////

  glm::ivec2 SWindow::GetWindowSize()
  {
    glm::ivec2 retVec;
    glfwGetWindowSize(window, &retVec.x, &retVec.y);
    return retVec;
  }
  
  ///////////////////////////////////////////////////////////////////
}
