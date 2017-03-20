/*
  SCamera.cc nachocpol@gmail.com
*/

#include "Engine/SCamera.h"
#include "Engine/SRenderManager.h"

namespace SE
{
  ///////////////////////////////////////////////////////////////////

  SCamera::SCamera()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SCamera::~SCamera()
  {
    
  }

  ///////////////////////////////////////////////////////////////////
  //If we initialize the camera before the rendermanager
  //we will retrieve wrong values
  void SCamera::Init(float fov, float near, float far)
  {
    curFov = fov;
    curNear = near;
    curFar = far;

    SRenderManager  *renderManager = SRenderManager::GetInstance();
    targetWindow = renderManager->GetWindow();

    Input = SInput::GetInstance();
    Input->Init(targetWindow);

    worldUp = glm::vec3(0, 1, 0);

    glm::vec2 ws = renderManager->GetWindowSize();
    float aspect = ws.x / ws.y;

    projection = glm::perspective(glm::radians(fov), aspect, near, far);
    position = glm::vec3(0, 0, 25);
  }

  ///////////////////////////////////////////////////////////////////

  glm::mat4* SCamera::GetView()
  {
    UpdateCameraVectors();
    view = glm::lookAt(position, position + cameraDirection, cameraUp);
    return &view;
  }

  ///////////////////////////////////////////////////////////////////

  glm::mat4* SCamera::GetProjection()
  {
    return &projection;
  }

  ///////////////////////////////////////////////////////////////////

  void SCamera::Move(glm::vec3 d)
  {
    position += d;
  }

  ///////////////////////////////////////////////////////////////////

  void SCamera::Update()
  {
    //TO-DO:dont call this each frame
    UpdateProjection();

    if (!importer)
    {
      if (!locked)
      {
        //Change speed using the delta time, with this we 
        //achieve a more consistent system
        float speed = cameraSpeed * SRenderManager::GetInstance()->GetDeltaTimeMs();

        if (Input->GetKey(kKeyW))
        {
          position += cameraDirection * speed;
        }
        if (Input->GetKey(kKeyS))
        {
          position -= cameraDirection * speed;
        }
        if (Input->GetKey(kKeyD))
        {
          position += cameraRight * speed;
        }
        if (Input->GetKey(kKeyA))
        {
          position -= cameraRight * speed;
        }
      }

      UpdateCamera();
      view = glm::lookAt(position, position + cameraDirection, cameraUp);
    }
    else
    {
      if (!locked)
      {
        float speed = cameraSpeed * SRenderManager::GetInstance()->GetDeltaTimeMs();

        if (Input->GetKey(kKeyW))
        {
          position += cameraDirection * speed;
        }
        if (Input->GetKey(kKeyS))
        {
          position -= cameraDirection * speed;
        }
      }
      view = glm::lookAt(position, position + cameraDirection, cameraUp);
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SCamera::SetCameraLock(bool isLocked)
  {
    locked = isLocked;
  }

  ///////////////////////////////////////////////////////////////////

  void SCamera::UpdateCamera()
  {
    //Only rotate the camera if we press right click
    if (glfwGetMouseButton(targetWindow, 1) == GLFW_PRESS)
    {
      double xpos, ypos;
      glfwGetCursorPos(targetWindow, &xpos, &ypos);
      
      if (firstUse)
      {
        lastX = xpos;
        lastY = ypos;
        firstUse = false;
      }

      GLfloat xoffset, yoffset;
      if (!locked)
      {
        xoffset = xpos - lastX;
        yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
      }
      else
      {
        xoffset = lastX - lastX;
        yoffset = lastY - lastY;
      }

      lastX = xpos;
      lastY = ypos;

      ProcessMouseMovement(xoffset, yoffset, true);
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SCamera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
  {
    xoffset *= this->sensitivity;
    yoffset *= this->sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
      //if (this->pitch > 89.0f)
        //this->pitch = 89.0f;
      //if (this->pitch < -89.0f)
        //this->pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->UpdateCameraVectors();
  }

  ///////////////////////////////////////////////////////////////////

  void SCamera::UpdateCameraVectors()
  {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->cameraDirection = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->cameraRight = glm::normalize(glm::cross(this->cameraDirection, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->cameraDirection));
  }

  ///////////////////////////////////////////////////////////////////

  void SCamera::UpdateProjection()
  {
    SRenderManager  *renderManager = SRenderManager::GetInstance();
    float aspect =  renderManager->GetWindowSize().x/ renderManager->GetWindowSize().y;

    projection = glm::perspective(glm::radians(curFov), aspect, curNear, curFar);
  }

  ///////////////////////////////////////////////////////////////////
}