/*
  SMesh.h nachocpol@gmail.com
*/

#ifndef SMESH_H
#define SMESH_H

#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "SComponent.h"
#include "SBuffer.h"


namespace SE
{
  class SMesh : public SComponent
  {
  public:
    SMesh();
    ~SMesh();

    /**
      Initializes this mesh object
      @param mc Parameters for the initialization
      @param forceClean If true it will generate the internal 
      buffers.
    */
    void Init(SMeshConstructor mc, bool forceClean = false);

    /**
      Initializes this mesh.This Init() method should be used to
      load user defined data into this Mesh.
      @param mc Parameters.
      @param forceClean If true it will generate the internal
    */
    void Init(SMeshConstructorProcedural mc, bool forceClean = false);

    /**
      Tells the GPU to render this mesh (remember to SMaterial::Use() before)
    */
    void MeshRender();

    /**
      Returns the GPU identifier of the mesh
      @return Returns the GPU identifier of the mesh
    */
    GLuint GetVao(){ return VAO; }

    const char* GetPath() { return path; };
    
  private:
    /**
      Checks if the mesh is loaded in the GPU
    */
    void CheckInternalState();

    /**
      Loads the model from disc
      @param path The path of the mesh
      @param dat2 Where should we save the mesh
    */
    void ImportSModel(const char* path, SBinaryModelData &dat2);
    
    SDrawMode drawMode;
    GLuint VAO;
    SBuffer* VBO;
    SBuffer* EBO;

    //This holds all the data of this mesh
    SBinaryModelData modelData;

    bool dirty = true;

    const char* path;
  };
}
#endif