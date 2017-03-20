/*
   STexture.cc nachocpol@gmail.com
*/

#include "Engine/STexture.h"
#include "Engine/SRenderManager.h"

namespace SE
{
  ///////////////////////////////////////////////////////////////////

  STexture::STexture()
  {

  }

  ///////////////////////////////////////////////////////////////////

  STexture::~STexture()
  {
    printf("Deleting the texture");
    glDeleteTextures(1, &id);
  }

  ///////////////////////////////////////////////////////////////////

  bool STexture::Init(const char *path)
  {
    int x, y,n;
    unsigned char *data = SRenderManager::GetInstance()->LoadTextureFromFile(path, x, y,n);

    //Check if data is valid
    if (data == nullptr)
    {
      std::cout << "Failed loading texture:" << std::endl;
      return false;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Upload data
    //TO-DO:2,3
    switch (n)  
    {
    case 1:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x, y, 0, GL_RED, GL_UNSIGNED_BYTE, data);
      break;
    case 4:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      break;

    default:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      break;
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    
    delete data;
    return true;
  }

  ///////////////////////////////////////////////////////////////////

  int STexture::GetId()
  {
    return (int)id;
  }

  ///////////////////////////////////////////////////////////////////

}