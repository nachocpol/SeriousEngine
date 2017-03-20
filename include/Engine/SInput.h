/*
SInput.h David Parra
*/


#ifndef SINPUT_H
#define SINPUT_H

#include <memory>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Engine/STypes.h"

namespace SE
{
  class SInput
  {
  public:
    static SInput *GetInstance();
    SInput();
    ~SInput();

    /**
      Saves the current window where the input is detected
      @param w The current window
    */
    void Init(GLFWwindow *w);

    /**
      Indicates if the specified key is pressed or not
      @param key The key that is going to be checked
      @return true if the key is currently being pressed false if not
    */
    bool GetKey(SInputKey key);

    /*
      Indicates if the specified key is released or not
      @param key The key that is going to be checked
      @return true if the key is currently being released false if not 
    */
    bool GetKeyRelease(SInputKey key);

  private:
    GLFWwindow *window;

    static SInput *instance;
  };
}
#endif