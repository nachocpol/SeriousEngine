/*
  SBaseScreenMaterial.h cortizopo@esat-alumni.com
*/

#ifndef SBASESCREENMATERIAL_H
#define SBASESCREENMATERIAL_H
#include "SMaterial.h"
namespace SE
{
  class SBaseScreenMaterial :public SMaterial
  {
  public:
    SBaseScreenMaterial();
    ~SBaseScreenMaterial();

    /*
      Initializes this screen material.
      @param vsPath the path to the vertex shader
      @param fsPath the path to the fragment shader
    */
    void Init(std::string vsPath, std::string fsPath);

    /*
      This material does not need a view or a projection,as it 
      will be rendered in actual screen coordinates
    */
    void Use(int texId);

  private:
    int screenTextureLoc;


  };
}
#endif