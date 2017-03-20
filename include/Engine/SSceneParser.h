/*
SSceneParser.h David Parra
*/

#ifndef SSCENEPARSER_H
#define SSCENEPARSER_H

#include "SGameObject.h"
#include <vector>

namespace SE
{
  class SSceneParser
  {
  public:
    /**
      Returns the instance of the Singleton
      @return The instance of the Singleton
    */
    static SSceneParser* GetInstance();

    /**
      Save the current state of the scene
      @param list The list of GameObjects of the scene
      @param savePath The path where the scene will be saved
    */
    void SaveScene(const std::vector<SGameObject *> &list, const char* savePath);

    /**
      Loads a scene from a file
      @param list The list of GameObjects that will be loaded
      @param loadPath The path where the scene will be loaded
      @param camPos The position of the current camera
    */
    void LoadScene(std::vector<SGameObject *> &list, const char* loadPath, glm::vec3 *camPos);

  private:
    SSceneParser();
    ~SSceneParser();
  };
}

#endif