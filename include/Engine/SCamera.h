/*
  SCamera.h nachocpol@gmail.com
*/
#ifndef SCAMERA_H
#define SCAMERA_H

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"


#include "SInput.h"


namespace SE
{
  class SCamera
  {
  public:
    SCamera();
    ~SCamera();

    /**
      Initializes this camera
      @param fov The field of view
      @param near The near plane
      @param far The far plane
    */
    void Init(float fov, float near, float far);

    /**
      Returns the view of this camera
      @param Returns the view of this camera
    */
    glm::mat4* GetView();

    /**
      Returns the projection of the camera
      @return Returns the projection of the camera
    */
    glm::mat4* GetProjection();

    glm::vec3 GetDirection() { return cameraDirection; }

    inline void SetPosition(glm::vec3 p) { position = p; }

    /*
      Moves the camera by the given vector.
    */
    void Move(glm::vec3 d);

    /**
      Updates the camera
    */
    void Update();

    /**
      Locks the camera 
    */
    void SetCameraLock(bool isLocked);

    /*
      Returns the position.
      @return returns the position
    */
    glm::vec3* GetPosition(){ return &position; }

    /*
      Returns the camera yaw.
      @return returns the camera yaw
    */
    float* GetYaw(){ return &yaw; }

    /*
      Returns the camera pitch.
      @return returns the camera pitch
    */
    float* GetPitch(){ return &pitch; }

    void SetImporter() { importer = true; }

  private:

    void UpdateCamera();
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void UpdateCameraVectors();
    void UpdateProjection();
    void OrbitCamera();

    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 position;
    glm::vec3 cameraDirection;
    glm::vec3 cameraTarget;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;
    glm::vec3 cameraUp;

    float yaw = -90;
    float pitch = 0;
    float sensitivity = 0.35f;

    bool locked = false;
    bool firstUse = true;
    double lastX;
    double lastY;
    GLFWwindow *targetWindow;

    float cameraSpeed = 0.035f;

    SInput *Input;

    float curFov = 0.0f;
    float curNear = 0.0f;
    float curFar = 0.0f;

    bool importer = false;
  };
}
#endif