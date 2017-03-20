/*
  SBuffer.cc nachocpol@gmail.com
*/

#include "Engine/SBuffer.h"
#include "Engine/SRenderManager.h"

namespace SE
{
  ///////////////////////////////////////////////////////////////////

  SBuffer::SBuffer():
    dirty(true)
  {
    SE::SRenderManager::GetInstance()->GenBuffer(1, id);
  }

  ///////////////////////////////////////////////////////////////////

  SBuffer::~SBuffer()
  {

  }

  ///////////////////////////////////////////////////////////////////

  void SBuffer::Init(SBufferType t, std::vector<float>& d, SUsage usage)
  {
    myType = t;
    myUsage = usage;
    fInternalBuffer = d;
    bufferSize = d.size() * sizeof(float);
    InternalPrepare();
  }

  ///////////////////////////////////////////////////////////////////

  void SBuffer::Init(SBufferType t, std::vector<unsigned int>& d, SUsage usage)
  {
    myType = t;
    myUsage = usage;
    uiInternalBuffer = d;
    bufferSize = d.size() * sizeof(unsigned int);
    InternalPrepare();
  }

  ///////////////////////////////////////////////////////////////////

  unsigned int SBuffer::GetId()
  {
    return id;
  }

  ///////////////////////////////////////////////////////////////////

  void SBuffer::CheckDirty()
  {
    if (dirty)
    {
      dirty = false;
      InternalReUpload();
    }
  }

  ///////////////////////////////////////////////////////////////////

  void SBuffer::UpdateBufferData()
  {
    
  }

  ///////////////////////////////////////////////////////////////////
  //This will only alloc space and wont upload any data
  void SBuffer::InternalPrepare()
  {
    SE::SRenderManager* renderManager = SE::SRenderManager::GetInstance();
    renderManager->BindBuffer(myType, id);
    //Upload unsigned int type
    if (uiInternalBuffer.size() > 0)
    {
      //renderManager->UploadBufferData(myType, uiInternalBuffer.size() * sizeof(unsigned int), &(*uiInternalBuffer.begin()), myUsage);
      renderManager->UploadBufferData(myType, uiInternalBuffer.size() * sizeof(unsigned int), NULL, myUsage);
      return;
    }
    //Upload unsigned float
    if (fInternalBuffer.size() > 0)
    {
      //renderManager->UploadBufferData(myType, fInternalBuffer.size() * sizeof(float), &(*fInternalBuffer.begin()), myUsage);
      renderManager->UploadBufferData(myType, fInternalBuffer.size() * sizeof(float), NULL, myUsage);
      return;
    }
    renderManager->UnbindBuffer(myType);
  }

  ///////////////////////////////////////////////////////////////////

  void SBuffer::InternalReUpload()
  {
    SE::SRenderManager* renderManager = SE::SRenderManager::GetInstance();
    renderManager->BindBuffer(myType, id);
    //Upload unsigned int type
    if (uiInternalBuffer.size() > 0)
    {
      renderManager->UploadSubData(myType, uiInternalBuffer.size() * sizeof(unsigned int), &(*uiInternalBuffer.begin()));
      return;
    }
    //Upload unsigned float
    if (fInternalBuffer.size() > 0)
    {
      renderManager->UploadSubData(myType, fInternalBuffer.size() * sizeof(float), &(*fInternalBuffer.begin()));
      return;
    }
    renderManager->UnbindBuffer(myType);
  }

  ///////////////////////////////////////////////////////////////////
}