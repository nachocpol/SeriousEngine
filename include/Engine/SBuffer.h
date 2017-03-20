/*
  SBuffer.h nachocpol@gmail.com
  This wont work if we make one buffer from a non-mainthread
  as we are calling opengl functions.
  Mesh is thread safe because when we init it we dont call any opengl
  we check it when we draw.
*/

#ifndef SBUFFER_H
#define SBUFFER_H

#include "STypes.h"

namespace SE
{
  class SBuffer
  {
  public:
    SBuffer();
    ~SBuffer();

    /**
      Initializes the buffer(float)
      @param t The type of the buffer
      @param d The data contained in the buffer
      @param usage How the buffer will be used
    */
    void Init(SBufferType t,std::vector<float>& d, SUsage usage);

    /**
      Initializes the buffer(unsigned int)
      @param t The type of the buffer
      @param d The data contained in the buffer
      @param usage How the buffer will be used
    */
    void Init(SBufferType t,std::vector<unsigned int>& d, SUsage usage);

    /**
      Returns the GPU buffer id
      @return Returns the GPU buffer id
    */
    unsigned int GetId();

    /**
      Checks if the GPU buffer is dirty (the data in the ram is not the 
      same that the data in the GPU) and uploads it
    */
    void CheckDirty();

    /**
      (To implement) We update a GPU buffer in real time.
    */
    void UpdateBufferData();

  private:
    /** 
      Prepares the GPU buffer but it does not upload the data,
      it just allocates the needed memory.
    */
    void InternalPrepare();

    /**
      Uploads the data to the GPU buffer.
    */
    void InternalReUpload();

    bool dirty;
    unsigned int id;
    unsigned int bufferSize;
    SBufferType myType;
    SUsage myUsage;
    
    std::vector<unsigned int> uiInternalBuffer;
    std::vector<float> fInternalBuffer;
  };
}
#endif SBUFFER_H