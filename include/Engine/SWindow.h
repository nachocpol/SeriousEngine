/*
  SWindow.h nachocpol@gmail.com
*/

#ifndef SWINDOW_H
#define SWINDOW_H

#include <memory>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"


namespace SE
{
  class SWindow
  {
  public:
    SWindow();
    ~SWindow();

    /**
      Initializes this window object
      @param w Width of the window
      @param h Height of the window
      @param title The title of the window
      @return True if successful,false if not
    */
    bool Init(int w, int h, const char* title);

    /**
      Delete the resources related with the window.
    */
    void Release();

    /**
      Returns the internal window object
      @return Internal window object
    */
    GLFWwindow* GetWindow();

    glm::ivec2 GetWindowSize();


  private:
    /**
      The window object
    */
    GLFWwindow  *window;
  };
}
#endif