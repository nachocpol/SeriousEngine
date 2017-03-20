/*
  STypes.h nachocpol@gmail.com & David Parra
  Defines types used by the engine.
  TO-DO:change the meshconstructor so we can pass the params in the constructor.
*/

#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include "glm.hpp"

namespace SE
{
  /**
    Drawing modes
  */
  enum SDrawMode
  {
    kTriangles  = 0,
    kLines      = 1
  };

  /**
    Type of buffer for the GPU
  */
  enum SBufferType
  {
    kArray      = 0,
    kElement    = 1
  };
  
  /**
    How the buffers will be used by the GPU
  */
  enum SUsage
  {
    KStaticDraw = 0
  };

  /**
    Types used by the engine
  */
  enum SDataType
  {
    KFloat      = 0,
    KInt        = 1,
    kMat3       = 2,
    kMat4       = 3,
    kFloat4     = 4,
    kFloat3     = 5,
    kFloat2     = 6
  };

  /**
   Type of vertex
  */
  enum SShaderType
  {
    kVertex     = 0,
    kFragment   = 1
  };

  /**
    Identifier of a key (used for input)
  */
  enum SInputKey
  {
    kKeyLeftArrow = 0,
    kKeyRightArrow,
    kKeyUpArrow,
    kKeyDownArrow,
    kKeyA,
    kKeyS,
    kKeyD,
    kKeyW,
    kKeyEscape,
    kKeyEnter,
    kKeySpace,
    kKeyR,
    kKeyL,
    kKeyO,
    kKeyF12
  };

  /**
    Used internaly by the rendering engine
  */
  typedef enum SCmdType
  {
    kUseMaterial = 0,
    kDraw = 1,
    kSetUniforms = 2
  };

  /*
    Data that represents a geometry
  */
  struct SBinaryModelData
  {
    unsigned int vertexSize = 0;
    unsigned int UVSize = 0;
    unsigned int normalSize = 0;
    unsigned int indicesSize = 0;
    unsigned int tangentsSize = 0;

    //positions-uvs-normals-tangents (must be in that order)
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
  };

  /**
    Used to initialize a SMesh.
  */
  struct SMeshConstructor
  {
    SDrawMode dMode;
    const char* path;
    SMeshConstructor(SDrawMode dM, const char* p)
    {
      dMode = dM;
      path = p;
    }
  };

  /*
    Mesh constructor for procedural geometry.
  */
  struct SMeshConstructorProcedural
  {
    SDrawMode dMode;
    SBinaryModelData modelData;
    SMeshConstructorProcedural(SDrawMode dM, SBinaryModelData mD)
    {
      dMode = dM;
      modelData = mD;
    }
  };

  /*
    Used to construct a screen quad defining the 4 vertices
  */
  struct SScreenQuadConstructor
  {
    /*
      Default constructor will make a full screen quad
    */
    SScreenQuadConstructor()
    {
      bl.x = -1;
      bl.y = -1;
      bl.z = 0.0f;

      br.x = 1;
      br.y = -1;
      br.z = 0.0f;

      tr.x = 1;
      tr.y = 1;
      tr.z = 0.0f;

      tl.x = -1;
      tl.y = 1;
      tl.z = 0.0f;
    }

    /*
      Custom size
    */
    SScreenQuadConstructor(glm::vec2 bl_, glm::vec2 br_, glm::vec2 tr_, glm::vec2 tl_)
    {
      bl.x = bl_.x;
      bl.y = bl_.y;
      bl.z = 0.0f;

      br.x = br_.x;
      br.y = br_.y;
      br.z = 0.0f;

      tr.x = tr_.x;
      tr.y = tr_.y;
      tr.z = 0.0f;

      tl.x = tl_.x;
      tl.y = tl_.y;
      tl.z = 0.0f;
    }

    glm::vec3 bl;
    glm::vec3 br;
    glm::vec3 tr;
    glm::vec3 tl;
  };

  /*
    Different lights types
  */
  enum SLightType
  {
    kSun = 0,
    kSpotLight = 1
  };

  /*
    Holds information about a light source
  */
  struct SLightInfo
  {
    SLightInfo()
    {

    }

    SLightInfo(SLightType type, glm::vec3 direction, glm::vec3 color, float near_ = 2.0f, float far_ = 50.0f, float size_ = 20.0f,float cutOff_ = 15.0f)
    {
      lType = type;
      lDir = direction;
      lCol = color;
      lNear = near_;
      lFar = far_;
      lSize = size_;
      lCutOff = cutOff_;
    };

    SLightType lType;
    glm::vec3 lDir;
    glm::vec3 lCol;
    float lNear, lFar, lSize,lCutOff;
  };

  /*
    Types of textures.
  */
  enum STextureType
  {
    k2DTexture      = 0,
    kTextureCubeMap = 1
  };
}
#endif