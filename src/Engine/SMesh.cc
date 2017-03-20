/*
  SMesh.cc nachocpol@gmail.com
*/

#include <fstream>

#include "Engine/SMesh.h"
#include "Engine/SDebug.h"
#include "Engine/SRenderManager.h"


namespace SE
{
  ///////////////////////////////////////////////////////////////////

  SMesh::SMesh()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SMesh::~SMesh()
  {
    
  }

  ///////////////////////////////////////////////////////////////////

  void SMesh::Init(SMeshConstructor mc, bool forceClean)
  {
    path = mc.path;
    ImportSModel(mc.path,modelData);
    drawMode = mc.dMode;
    if (forceClean)
    {
      CheckInternalState();
      VBO->CheckDirty();
      EBO->CheckDirty();
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SMesh::Init(SMeshConstructorProcedural mc, bool forceClean)
  {
    modelData = mc.modelData;
    drawMode = mc.dMode;
    if (forceClean)
    {
      CheckInternalState();
      VBO->CheckDirty();
      EBO->CheckDirty();
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SMesh::MeshRender()
  {
    SRenderManager *renderManager = SRenderManager::GetInstance();
    //Check if this mesh is dirty
    if (dirty)
    {
      //Perform internal GPU operations
      CheckInternalState();
    }
    VBO->CheckDirty();
    EBO->CheckDirty();
    renderManager->Draw(drawMode, modelData.indices.size(), VAO);
    renderManager->GetAndPrintError("Drawing a mesh.",false);
  }

  ///////////////////////////////////////////////////////////////////

  void SMesh::CheckInternalState()
  {
    //Save RenderManager instance
    SRenderManager *renderManager = SRenderManager::GetInstance();

    //generate buffers
    renderManager->GenVertexArrayObj(1, VAO);
    VBO = new SBuffer();
    EBO = new SBuffer();

    renderManager->BindVertexArray(VAO);
    {
      //bind array buffer&upload data
      renderManager->BindBuffer(kArray, VBO->GetId());
      VBO->Init(kArray, modelData.vertices, KStaticDraw);

      //bind indices buffer&upload data
      renderManager->BindBuffer(kElement, EBO->GetId());
      EBO->Init(kElement, modelData.indices, KStaticDraw);

      //location(0)(positions)
      if (modelData.vertexSize > 0)
      {
        renderManager->SetVertAttribPoint(0, 3, KFloat, false, 3 * sizeof(GLfloat), 0);
      }

      //location(1)(uvs)
      if (modelData.UVSize > 0)
      {
        renderManager->SetVertAttribPoint(1, 2, KFloat, false, 2 * sizeof(GLfloat), modelData.vertexSize * sizeof(GLfloat));
      }

      //location(2)(normals)
      if (modelData.normalSize > 0)
      {
        renderManager->SetVertAttribPoint(2, 3, KFloat, false, 3 * sizeof(GLfloat), (modelData.UVSize + modelData.vertexSize) * sizeof(GLfloat));
      }

      //location(3)(tangents)
      if (modelData.tangentsSize > 0)
      {
        renderManager->SetVertAttribPoint(3, 3, KFloat, false, 3 * sizeof(GLfloat), (modelData.normalSize + modelData.UVSize + modelData.vertexSize) * sizeof(GLfloat));
      }
    }
    renderManager->UnbindVertexArray();

    dirty = false;
  }

  ///////////////////////////////////////////////////////////////////

  void SMesh::ImportSModel(const char *path, SE::SBinaryModelData &dat2)
  {
    std::ifstream readFile;
    readFile.open(path, std::ios::in | std::ios::binary);
    if (readFile)
    {
      //Read the number of vertices (a vertex has 3 elements(x,y,z))
      readFile.read(reinterpret_cast<char*>(&dat2.vertexSize), sizeof(unsigned int));
      //Read the number of normals (a normal has 3 elements(x,y,z))
      readFile.read(reinterpret_cast<char*>(&dat2.normalSize), sizeof(unsigned int));
      //Read the number of UVs (a UV has 2 elements(w,h))
      readFile.read(reinterpret_cast<char*>(&dat2.UVSize), sizeof(unsigned int));
      readFile.read(reinterpret_cast<char*>(&dat2.tangentsSize), sizeof(unsigned int));
      //Read the number of indices (a index has 3 elements(1, 2, 3))
      readFile.read(reinterpret_cast<char*>(&dat2.indicesSize), sizeof(unsigned int));

      //Set the position of the reading pointer after the previous variable 
      //readFile.seekg(sizeof(int));

      //Resize the number of elements of the vector to save the read values
      dat2.vertices.resize(dat2.vertexSize + dat2.UVSize + dat2.normalSize + dat2.tangentsSize);
      readFile.read(reinterpret_cast<char*>(&dat2.vertices[0]), (dat2.vertexSize + dat2.UVSize + dat2.normalSize + dat2.tangentsSize) * sizeof(float));
      dat2.indices.resize(dat2.indicesSize);
      readFile.read(reinterpret_cast<char*>(&dat2.indices[0]), dat2.indicesSize * sizeof(unsigned int));
    }
    readFile.close();
  }

  ///////////////////////////////////////////////////////////////////
}