/*
SInput.h David Parra
*/

#include "Engine/SInput.h"

int glfw_keys[] = {
  GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_A, GLFW_KEY_S,
  GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_ESCAPE, GLFW_KEY_ENTER, GLFW_KEY_SPACE , GLFW_KEY_R, GLFW_KEY_L,
  GLFW_KEY_O, GLFW_KEY_F12
};

namespace SE
{
  SInput* SInput::instance = NULL;

  ///////////////////////////////////////////////////////////////////

  SInput *SInput::GetInstance()
  {
    if (!instance)
    {
      instance = new SInput;
    }
    return instance;
  }
  ///////////////////////////////////////////////////////////////////

  SInput::SInput()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SInput::~SInput()
  {

  }

  ///////////////////////////////////////////////////////////////////

  void SInput::Init(GLFWwindow *w)
  {
    window = w;
  }

  ///////////////////////////////////////////////////////////////////

  bool SInput::GetKey(SInputKey key)
  {
    if (glfwGetKey(window, glfw_keys[key]) == GLFW_PRESS)
    {
      return true;
    }
    return false;
  }

  ///////////////////////////////////////////////////////////////////

  bool SInput::GetKeyRelease(SInputKey key)
  {
    if (glfwGetKey(window, glfw_keys[key]) == GLFW_RELEASE)
    {
      return true;
    }
    return false;
  }

  ///////////////////////////////////////////////////////////////////
}