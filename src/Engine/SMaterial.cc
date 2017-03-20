/*
  SMaterial.cc nachocpol@gmail.com
*/

#include "Engine/SMaterial.h"
#include "Engine/SRenderManager.h"
#include "Engine/SResourceManager.h"
#include "Engine/SLight.h"

namespace SE
{
  ///////////////////////////////////////////////////////////////////

  SMaterial::SMaterial()
  {

  }

  ///////////////////////////////////////////////////////////////////

  SMaterial::~SMaterial()
  {
    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);
    glDeleteProgram(programId);
  }

  ///////////////////////////////////////////////////////////////////

  void SMaterial::Init(SMaterialSettings* matSet)
  {
    //Variables
    GLint success;
    GLchar infoLog[512];
    memset(infoLog, '/0', sizeof(infoLog));

    //Init shaders
    InitShaders(matSet->vsp, matSet->fsp);

    //Init program
    programId = glCreateProgram();
    glAttachShader(programId, vertShaderId);
    glAttachShader(programId, fragShaderId);
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(programId, 512, NULL, infoLog);
      std::cout << "Vertex:" << matSet->vsp << std::endl;
      std::cout << "Fragment:" << matSet->fsp << std::endl;
      std::cout << "Failed to link program.\n" << infoLog << std::endl;
    }

    rendMang = SE::SRenderManager::GetInstance();

    //Get default locations
    modelLocation = rendMang->GetUniform(programId, "uModel");
    shadowMapLocation = rendMang->GetUniform(programId, "uShadowMap");

    //Init the material settings
    matSettings = matSet;
    matSettings->GetLocations(programId);

    //Set draw data bindings (view,projection) to the position 0
    glUniformBlockBinding(programId,glGetUniformBlockIndex(programId,"uDrawData"),0);
    //Set light data bindings (shadowmap,lights,lightmatrix) to the position 1
    //glUniformBlockBinding(programId, glGetUniformBlockIndex(programId, "uLightData"), 1);
    //Bind fog data to position 2
    glUniformBlockBinding(programId, glGetUniformBlockIndex(programId, "uFogData"), 2);

#ifdef PERFORM_DEBUG
    ShowDebug();
#endif
  }

  ///////////////////////////////////////////////////////////////////

  void SMaterial::Reload()
  {
    //Variables
    GLint success;
    GLchar infoLog[512];
    memset(infoLog, '/0', sizeof(infoLog));

    //Init shaders
    InitShaders(matSettings->vsp, matSettings->fsp,true);

    //Init program
    glDeleteProgram(programId);
    programId = glCreateProgram();
    glAttachShader(programId, vertShaderId);
    glAttachShader(programId, fragShaderId);
    glLinkProgram(programId);
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(programId, 512, NULL, infoLog);
      std::cout << "Vertex:" << matSettings->vsp << std::endl;
      std::cout << "Fragment:" << matSettings->fsp << std::endl;
      std::cout << "Failed to reload shader program:\n" << infoLog << std::endl;
      system("pause");
    }

    //Get default locations
    modelLocation = rendMang->GetUniform(programId, "uModel");
    shadowMapLocation = rendMang->GetUniform(programId, "uShadowMap");

    matSettings->GetLocations(programId);

    glUniformBlockBinding(programId,glGetUniformBlockIndex(programId,"uDrawData"),0);
    glUniformBlockBinding(programId, glGetUniformBlockIndex(programId, "uFogData"), 2);
  }

  ///////////////////////////////////////////////////////////////////

  void SMaterial::Use()
  {
    //Use the material
    glUseProgram(programId);

    //Set the attributes of the material settings
    matSettings->SetLocations(programId);
  }

  ///////////////////////////////////////////////////////////////////

  void SMaterial::Use(glm::mat4 &m, std::vector<SLight*> &l)
  {
    //Use the material
    glUseProgram(programId);

    //Set the defaul uniforms,the material settings could request to dont do so
    if (matSettings->setDefaultAttributes)
    {
      rendMang->SetUniform(programId, "uModel", kMat4, glm::value_ptr(m), modelLocation);

      //Set lights
      int cLightsLoc = rendMang->GetUniform(programId, "uCurLights");
      glUniform1i(cLightsLoc, l.size());

      for (unsigned int i = 0; i < l.size(); i++)
      {
        //Set color and type
        std::string lColName = "uLights[" + std::to_string(i) + "].lColor";
        glm::vec4 lCol = glm::vec4( l[i]->GetLightInfo().lCol.x,
                                    l[i]->GetLightInfo().lCol.y,
                                    l[i]->GetLightInfo().lCol.z,
                                    l[i]->GetLightType());
        rendMang->SetUniform(programId, lColName.c_str(), kFloat4, &lCol.x,
                             rendMang->GetUniform(programId, lColName.c_str()));

        //Sun light
        if (l[i]->GetLightInfo().lType == kSun)
        {
          //Set light direction
          std::string lDirName = "uLights[" + std::to_string(i) + "].lDir";
          rendMang->SetUniform(programId, lDirName.c_str(), kFloat3, glm::value_ptr(l[i]->GetLightInfo().lDir),
                               rendMang->GetUniform(programId, lDirName.c_str()));

          //Set light matrix
          std::string lMatName = "uLights[" + std::to_string(i) + "].lMatrix";
          rendMang->SetUniform(programId, lMatName.c_str(), kMat4, glm::value_ptr(l[i]->GetLightMatrix()),
                               rendMang->GetUniform(programId, lMatName.c_str()));
        }
        
        //Spot light
        if (l[i]->GetLightInfo().lType == kSpotLight)
        {
          //Set light direction
          std::string lDirName = "uLights[" + std::to_string(i) + "].lDir";
          rendMang->SetUniform(programId, lDirName.c_str(), kFloat3, glm::value_ptr(l[i]->GetLightInfo().lDir),
                               rendMang->GetUniform(programId, lDirName.c_str()));

          //Set light matrix
          std::string lMatName = "uLights[" + std::to_string(i) + "].lMatrix";
          rendMang->SetUniform(programId, lMatName.c_str(), kMat4, glm::value_ptr(l[i]->GetLightMatrix()),
                               rendMang->GetUniform(programId, lMatName.c_str()));

          //Set light position
          std::string lPosName = "uLights[" + std::to_string(i) + "].lPosition";
          rendMang->SetUniform(programId,lPosName.c_str(),kFloat3,glm::value_ptr(*l[i]->GetPosition()),
                                rendMang->GetUniform(programId, lPosName.c_str()));

          //Set light cut off
          float cOff = glm::cos(glm::radians(l[i]->GetLightInfo().lCutOff));
          std::string lCutName = "uLights[" + std::to_string(i) + "].lCutOff";
          rendMang->SetUniform( programId,lCutName.c_str(),KFloat,&cOff,
                                rendMang->GetUniform(programId,lCutName.c_str()));
        }
      }

      //Set the shadow maps
      for (unsigned int i = 0; i < l.size(); i++)
      {
        std::string lShadowMap = "uShadowMaps[" + std::to_string(i) + "]";
        rendMang->SetTexture(20 + i, k2DTexture, l[i]->GetShadowTexure(),
                             rendMang->GetUniform(programId, lShadowMap.c_str()));
      }
    }

    //Set the attributes of the material settings
    matSettings->SetLocations(programId);
  }

  ///////////////////////////////////////////////////////////////////

  void SMaterial::Use(glm::mat4 lm, glm::mat4 &m)
  {
    //Use the material
    glUseProgram(programId);

    //Set the defaul uniforms,the material settings could request to dont do so
    if (matSettings->setDefaultAttributes)
    {
      rendMang->SetUniform(programId, "uModel", kMat4, glm::value_ptr(m), modelLocation);
    }

    //Set the attributes of the material settings
    SShadowMaterialSet* sMatSet = (SShadowMaterialSet*)matSettings;
    sMatSet->SetLocations(programId, lm);
  }

  ///////////////////////////////////////////////////////////////////

  void SMaterial::CreateMaterialSettings(SCamera* sceneCam)
  {
    SPhysicalMatSet* tmpMatSet = nullptr;
    tmpMatSet = new SPhysicalMatSet(
      "../data/shaders/seriousPS.vs", "../data/shaders/seriousPS.fs",
      "../data/img/default.png", "../data/img/default.png", "../data/img/default.png","../data/img/default.png");
    tmpMatSet->refCamPos = sceneCam->GetPosition();
    matSettings = tmpMatSet;
  }

  ///////////////////////////////////////////////////////////////////

  void SMaterial::InitShaders(const char* vpath, const char* fpath,bool reload)
  {
    //If relaod clean old shaders
    if(reload)
    {
      glDeleteShader(vertShaderId);
      glDeleteShader(fragShaderId);
      //...
    }

    GLint success;
    GLchar infoLog[512];
    char *code;

    memset(infoLog, 0, sizeof(infoLog));


    //vertext
    LoadFromFile(vpath, code);
    vertShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShaderId, 1, &code, NULL);
    glCompileShader(vertShaderId);

    glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragShaderId, 512, NULL, infoLog);
      //std::cout << "Fragment:" << matSettings->fsp << std::endl;
      std::cout << "Vertex shader failed to compile:\n" << infoLog << std::endl;
      system("pause");
    }

    //fragment
    LoadFromFile(fpath, code);
    fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShaderId, 1, &code, NULL);
    glCompileShader(fragShaderId);

    glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragShaderId, 512, NULL, infoLog);
      //std::cout << "Vertex:" << matSettings->vsp << std::endl;
      std::cout << "Fragment shader failed to compile:\n" << infoLog << std::endl;
      system("pause");
    }
    //...
  }

  ///////////////////////////////////////////////////////////////////

  void SMaterial::LoadFromFile(const char* path, char *&out)
  {
    FILE * tmpFile;
    int size;

    //Load file
    fopen_s(&tmpFile,path, "rb");

    if (tmpFile == NULL)
    {
      std::cout << "Cant open shader file:" << path << std::endl;
      return;
    }

    //Get file size
    fseek(tmpFile, 0, SEEK_END);
    size = ftell(tmpFile);
    rewind(tmpFile);

    //Alloc out char
    out = new char[size];
    out[size] = 0;
    memset(out, '/0', size);

    //Copy data into the out char
    fread(out, size, 1, tmpFile);

    fclose(tmpFile);
  }

  ///////////////////////////////////////////////////////////////////

  void SMaterial::ShowDebug()
  {
    //Get active atributes
    //We could store this to give info to mesh::Draw
    GLint maxLength, nAttribs;
    glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES,&nAttribs);
    glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,&maxLength);
    GLchar * name = (GLchar *)malloc(maxLength);

    GLint written, size, location;
    GLenum type;

    printf("\n Attributes:\n");
    printf(" Index | Name\n");
    printf("-------------------------\n");
    for (int i = 0; i < nAttribs; i++)
    {
      glGetActiveAttrib(programId, i, maxLength, &written, &size, &type, name);
      location = glGetAttribLocation(programId, name);
      printf(" %-5d | %s\n", location, name);
    }
    free(name);

    //Get active uniforms
    GLint nUniforms;
    maxLength = 0;
    glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH,&maxLength);
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS,&nUniforms);
    GLchar *uname = (GLchar *)malloc(maxLength);

    printf("\n Uniforms:\n");
    printf(" Location | Name\n");
    printf("-------------------------\n");
    for (int i = 0; i < nUniforms; ++i)
    {
      glGetActiveUniform(programId, i, maxLength, &written, &size, &type, name);
      location = glGetUniformLocation(programId, uname);
      printf(" %-8d | %s\n", location, uname);
    }
    free(uname);
  }

  ///////////////////////////////////////////////////////////////////
}