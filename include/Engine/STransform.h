/*
  STransform.h cortizopo@esat-alumni.com
*/

#ifndef STransform_H
#define STransform_H

#include "SComponent.h"

#include "glm.hpp"

namespace SE
{
  class SGameObject;

  class STransform :public SComponent
  {
  public:
    STransform();
    ~STransform();

    void SetGoOwner(SGameObject* oGo);
    /*
      Updates this transform component.
    */
    void Update();

    /*
      Returns the position.
      @return Returns the position.
    */
    glm::vec3* GetPosition();

    /*
      Returns the rotation.
      @return Returns the rotation.
    */
    glm::vec3* GetRotation();

    /*
      Returns the scale.
      @return Returns the scale.
    */
    glm::vec3* GetScale();

    /*
      Returns the model matrix.
      @return Returns the model matrix.
    */
    glm::mat4* GetModel();

    /**
      Adds a child to this Game Object.
      @param go The game object to be attached
    */
    void AddChild(SGameObject* go);

    /*
      Attachs this game objet to another.
      @param go The game object that we are attached to
    */
    void AttachTo(SGameObject* go);

    /**
      @return Returns the child of this Game Object
    */
    SGameObject* GetChild();

    /**
      Returns our parent if we dont have we return nullptr
      @return Returns our parent if we dont have we return nullptr
    */
    SGameObject* GetParent();

    /**
      Removes the child of this game object, that does not mean
      that the child looses the reference to the parent so
      you should call "DettachFromParent" at the child.
    */
    void RemoveChild();

    /**
      Dettachs this game object from its parent.
      @param keepTrans Should we keep our current transformation?
    */
    void DettachFromParent(bool keepTrans);

    /**
      Moves the object
      @param v Translates the game object
    */
    void Translate(glm::vec3 &v);

    /**
      Rotates the object
      @param v The vector to rotate the object
    */
    void Rotate(glm::vec3 &v);

    /**
      Scales the object
      @param v The scale vector
    */
    void Scale(glm::vec3 &v);

    /**
      Overrides the object current position
      @param p The new position of the game object
    */
    void SetPosition(glm::vec3 &p);

    /**
      Overrides the object rotation.
      @param v The new rotation of the game object
    */
    void SetRotation(glm::vec3 &v);

    /**
      Overrides the current scale
      @param v The new scale of the game object
    */
    void SetScale(glm::vec3 &v);

  private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 modelL;
    glm::mat4 modelG;

    //A reference to the game object that owns us
    SGameObject* thisGO = nullptr;
    SGameObject* parent = nullptr;
    SGameObject* child  = nullptr;

    bool isDirty = true;
  };
}
#endif