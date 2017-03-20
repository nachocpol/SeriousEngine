#ifndef SMISCENE_H
#define SMISCENE_H

#include "Engine/SScene.h"

namespace SE
{
  class SMIScene : public SScene
  {
  public:
    SMIScene();
    ~SMIScene();

  private:
    void Render() override;
    void RenderGUI() override;
    bool LoadModel(const char *path);
    void ASSIMPLoad(const char* p);
    void ExportSModel(const char *path, SE::SBinaryModelData &dat);

    unsigned int curUvCnt = 0;

    int numVertex = 0;
    int numUV = 0;
    int numTangents = 0;

    bool overrideUvs = false;

    //SE::SPhysicalMatSet* matSettings;
    SE::SBinaryModelData* data = nullptr;

    int modelID = -1;
  };
}

#endif