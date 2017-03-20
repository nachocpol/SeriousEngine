/*
  STransform.cc cortizopo@esat-alumni.com
*/

#include "Engine/STransform.h"
#include "Engine/SGameObject.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/matrix_decompose.hpp"

namespace SE
{
  ////////////////////////////////////////////////

  STransform::STransform()
  {
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
  }

  ////////////////////////////////////////////////

  STransform::~STransform()
  {

  }

  ////////////////////////////////////////////////

  void STransform::SetGoOwner(SGameObject* oGo)
  {
    if (oGo == nullptr)
    {
      std::cout << "STransform():ERROR,passed a null game object.\n";
      return;
    }
    thisGO = oGo;
  }

  ////////////////////////////////////////////////

  void STransform::Update()
  {
    //Only update the transform if it is dirty
    if (isDirty)
    {
      //Reset model
      modelL = glm::mat4(1.0f);

      glm::mat4 posMat, rotMat, scaleMat;

      //Calculate each single transformation
      posMat = glm::translate(posMat, position);
      rotMat = glm::rotate(rotMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0, 0.0f));
      rotMat = glm::rotate(rotMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0, 0.0f));
      rotMat = glm::rotate(rotMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0, 1.0f));
      scaleMat = glm::scale(scaleMat, scale);

      //Calculate local matrix
      modelL = posMat * rotMat * scaleMat;

      isDirty = false;
    }

    //We have a parent so recal our global
    if (parent != nullptr)
      modelG = *parent->transform->GetModel() * modelL;

    //We dont have a parent so our global is our local
    else
      modelG = modelL;
  }

  ////////////////////////////////////////////////

  glm::vec3* STransform::GetPosition()
  {
    return &position;
  }

  ////////////////////////////////////////////////

  glm::vec3* STransform::GetRotation()
  {
    return &rotation;
  }

  ////////////////////////////////////////////////

  glm::vec3* STransform::GetScale()
  {
    return &scale;
  }


  ////////////////////////////////////////////////
  
  glm::mat4* STransform::GetModel()
  {
    return &modelG;
  }

  ////////////////////////////////////////////////

  void STransform::AddChild(SGameObject *go)
  {
    //To avoid infinite loop
    if (child == go)
    {
      return;
    }

    child = go;
    child->transform->AttachTo(thisGO);
  }

  ////////////////////////////////////////////////

  void STransform::AttachTo(SGameObject *go)
  {
    //To avoid infinite loop
    if (parent == go)
    {
      return;
    }

    parent = go;

    //Make our position local to the parent GO
    glm::mat4 modelInv = glm::inverse(*parent->transform->GetModel());
    glm::mat4 globToLocmat = modelInv * modelG;
    position  = glm::vec3(globToLocmat[3]);

    //We decompose the globalToLocalmat
    glm::vec3 dScale, dTrans, dSkew;
    glm::quat dQuat;
    glm::vec4 dPres;
    glm::decompose(globToLocmat, dScale, dQuat, dTrans, dSkew, dPres);

    rotation.x = glm::degrees(dQuat.x);
    rotation.y = glm::degrees(dQuat.y);
    rotation.z = glm::degrees(dQuat.z);

    parent->transform->AddChild(thisGO);

    //TO-DO:check if this is correct
    isDirty = true;
  }

  ////////////////////////////////////////////////

  SGameObject* STransform::GetChild()
  {
    return child;
  }


  ////////////////////////////////////////////////

  SGameObject* STransform::GetParent()
  {
    return parent;
  }

  ////////////////////////////////////////////////

  void STransform::RemoveChild()
  {
    //Sanity check
    if (child == nullptr)
    {
      return;
    }
    child = nullptr;
  }

  ////////////////////////////////////////////////

  void STransform::DettachFromParent(bool keepTrans)
  {
    //Sanity check
    if (parent == nullptr)
    {
      return;
    }
    if (keepTrans)
    {
      //Keep the position and the rotation
      position  = glm::vec3(modelG[3]);
      rotation = parent->transform->rotation;
    }
    else
    {
      //¿?
    }

    parent = nullptr;

    //TO-DO:check if this is correct
    isDirty = true;
  }

  ////////////////////////////////////////////////

  void STransform::Translate(glm::vec3 &v)
  {
    position += v;
    isDirty = true;
  }

  ////////////////////////////////////////////////

  void STransform::Rotate(glm::vec3 &v)
  {
    rotation += v;
    isDirty = true;
  }

  ////////////////////////////////////////////////

  void STransform::Scale(glm::vec3 &v)
  {
    scale += v;
    isDirty = true;
  }

  ////////////////////////////////////////////////

  void STransform::SetPosition(glm::vec3 &p)
  {
    position = p;
    isDirty = true;
  }

  ////////////////////////////////////////////////

  void STransform::SetRotation(glm::vec3 &v)
  {
    rotation = v;
    isDirty = true;
  }

  ////////////////////////////////////////////////

  void STransform::SetScale(glm::vec3 &v)
  {
    scale = v;
    isDirty = true;
  }

  ////////////////////////////////////////////////
}