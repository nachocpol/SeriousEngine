/*
  SGameObject.h nachocpol@gmail.com & David Parra
*/

#ifndef SGAMEOBJECT_H
#define SGAMEOBJECT_H

#include "SComponentCache.h"

namespace SE
{
  class STransform;
  class SLight;
  class SScript;
  class SMaterial;
  class SMesh;

  class SGameObject
  {
  public:
    SGameObject();
    ~SGameObject();

    /**
      Initializes some internal values.
      @param mc Properties of the game object
    */
    void Init(std::string goName);

    /**
      Updates the model transformation
    */
    void Update();

    /*
      Adds a new component of the given type to the
      game object.
    */
    template <typename C>
    void AddComponent();

    /*
      Add component for the mesh component.
    */
    template <typename C>
    bool AddComponent(SMeshConstructor mc);

    template <typename C>
    void RemoveComponent();

    /*
      Returns the component of the specified type.
      @return Returns the component of the specified type.
    */
    SComponent* GetComponent(std::string compType);

    /*
      Delete the GameObject with all of his components.
    */
    void DeleteGameObject();

    std::string myName;

    //References to important components
    //NOTE:If they are nullptr it will boom!!!
    SMesh* mesh = nullptr;
    SMaterial* material = nullptr;
    STransform* transform = nullptr;
    SScript* script = nullptr;
    SLight* light = nullptr;

  private:
    SComponentCache* compCache;
  };
}
#endif