//***************************************************************************//
// Scene Class Implementation
//
// Created Sept 04, 2005
// By: Jeremy Michael Miller
//
// Copyright (c) 2005-2016 Jeremy Michael Miller. 
// Author: = "Jeremy Michael Miller"
// Copyright: = "Copyright 2005-2016, Graphical Miller,  All rights reserved."
// Credits = ["Jeremy Michael Miller"]
// License: "Fair use v0.9"
// Version: "0.0.1"
// Maintainer: "Jeremy Michael Miller"
// Email: "maybe_later@mst.dnsalias.net"
// Status: "Alpha"
//***************************************************************************//

//***************************************************************************//
// System Includes
#include <assert.h>
#include <fstream>
#include <string>
#include <sstream>
//***************************************************************************//

//***************************************************************************//
// Platform Includes
#include <Windows.h>
#include <glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
//***************************************************************************//

//***************************************************************************//
// Local Includes
#include "Camera.hpp"
#include "Light.hpp"
#include "GraphicalMiller.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "TGA.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
//***************************************************************************//

//***************************************************************************//
#define SHADER_LABEL   "Shader"
#define SCENE_LABEL    "Scene"
#define LIGHT_LABEL    "Light"
#define MATERIAL_LABEL "Material"
#define TEXTURE_LABEL  "Texture"
#define MESH_LABEL     "Mesh"
#define CAMERA_LABEL   "Camera"
#define NAME_LABEL     "Name"
#define TYPE_LABEL     "Type"
//***************************************************************************//

//***************************************************************************//
using namespace std;
//***************************************************************************//

//***************************************************************************//
namespace MST
{
  //*************************************************************************//
  // Start of class GeometryBuffer implementation
  //-------------------------------------------------------------------------//
  GeometryBuffer::GeometryBuffer(const class Graphics* const pGraphics)
  {
    Create(pGraphics);
  }
  //-------------------------------------------------------------------------//

  GeometryBuffer::GeometryBuffer(const GeometryBuffer& rSource)
  {
    Create(rSource.mpGraphics);
    Clone(rSource);
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  GeometryBuffer::~GeometryBuffer()
  {
    try
    {
      Destroy();
    }
    catch(...)
    {

    }
  }
  //-------------------------------------------------------------------------//

  //-----------------------------------------------------------------------//
  void GeometryBuffer::Create(const class Graphics* const pGraphics)
  {
    this->mpGraphics = pGraphics;

    this->muiIndexBufferID = 0;
    this->muiVertexBufferID = 0;

    this->muiMaxVertexSize = this->mpGraphics->MaxVertexBufferSize();
    this->muiMaxIndexSize = this->mpGraphics->MaxIndexBufferSize();

    this->mulIndexFreeElements  = this->muiMaxIndexSize;
    this->mulVertexFreeElements = this->muiMaxVertexSize;
  }
  //-----------------------------------------------------------------------//

  //-----------------------------------------------------------------------//
  void GeometryBuffer::Clone(const GeometryBuffer& rSource)
  {
    //-----------------------------------------------------------------------//
    this->mpGraphics = rSource.mpGraphics;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    this->muiVertexBufferID = rSource.muiVertexBufferID;
    this->muiIndexBufferID = rSource.muiIndexBufferID;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    this->mulVertexFreeElements = rSource.mulVertexFreeElements;
    this->mulIndexFreeElements = rSource.mulIndexFreeElements;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    this->muiMaxVertexSize = rSource.muiMaxVertexSize;
    this->muiMaxIndexSize = rSource.muiMaxIndexSize;
    //-----------------------------------------------------------------------//
  }
  //-------------------------------------------------------------------------//

  //-----------------------------------------------------------------------//
  void GeometryBuffer::Destroy()
  {
    if(this->muiIndexBufferID)
      glDeleteBuffersARB(1, &this->muiIndexBufferID);

    if(this->muiVertexBufferID)
      glDeleteBuffersARB(1, &this->muiVertexBufferID);
  }
  //-----------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  unsigned long GeometryBuffer::VertexFreeElements() const
  {
    return this->mulVertexFreeElements;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  unsigned long GeometryBuffer::IndexFreeElements() const
  {
    return this->mulIndexFreeElements;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  unsigned long  GeometryBuffer::MaxVertexElements() const
  {
    return this->muiMaxVertexSize;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  unsigned long  GeometryBuffer::MaxIndexElements() const
  {
    return this->muiMaxIndexSize;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  unsigned long  GeometryBuffer::UsedVertexElements() const
  {
    return this->muiMaxVertexSize - this->mulVertexFreeElements;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  unsigned long  GeometryBuffer::UsedIndexElements() const
  {
    return this->muiMaxIndexSize - this->mulIndexFreeElements;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  unsigned long GeometryBuffer::Allocate(const class Vertex* const pVertices, unsigned long ulVerticesLength, 
    const unsigned long* const pIndices, unsigned long ulIndicesLength)
  {
    unsigned long ulReturnValue = ULONG_MAX;

    if((this->mulIndexFreeElements >= ulIndicesLength) && 
      (this->mulVertexFreeElements >= ulVerticesLength))
    {

      if(this->muiIndexBufferID == 0 && this->muiVertexBufferID == 0)
      {
        glGenBuffersARB(1, static_cast<GLuint*>(&muiVertexBufferID));
        glGenBuffersARB(1, static_cast<GLuint*>(&muiIndexBufferID));

        glBindBufferARB(GL_ARRAY_BUFFER, this->muiVertexBufferID);
        glBufferDataARB(GL_ARRAY_BUFFER, sizeof(Vertex) * muiMaxVertexSize, NULL, GL_STATIC_DRAW_ARB);

        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, this->muiIndexBufferID); 
        glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, muiMaxIndexSize * sizeof(unsigned long), NULL, GL_STATIC_DRAW_ARB);
      }

      glBindBufferARB(GL_ARRAY_BUFFER, this->muiVertexBufferID);
      glBufferSubDataARB(GL_ARRAY_BUFFER, sizeof(Vertex) * (this->muiMaxVertexSize - this->mulVertexFreeElements), sizeof(Vertex) * ulVerticesLength, pVertices);

      this->mulVertexFreeElements -= ulVerticesLength;

      Vertex::DeclareVertex();

      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, this->muiIndexBufferID); 
      ulReturnValue = this->muiMaxIndexSize - this->mulIndexFreeElements;
      glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER, ulReturnValue * sizeof(unsigned long), ulIndicesLength * sizeof(unsigned long), pIndices);

      this->mulIndexFreeElements -= ulIndicesLength;

      glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);  
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    }

    return ulReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  void GeometryBuffer::Bind()
  {
    glBindBufferARB(GL_ARRAY_BUFFER, this->muiVertexBufferID);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, this->muiIndexBufferID);
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  void GeometryBuffer::Unbind()
  {
    glBindBufferARB(GL_ARRAY_BUFFER, 0);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  //-------------------------------------------------------------------------//
  // End of class GeometryBuffer implementation
  //*************************************************************************//

  //*************************************************************************//
  Scene::Scene(class Graphics* pGraphics)
  {
    this->mpGraphics = pGraphics;
    this->muiShaderProgram = 0;
  }
  //*************************************************************************//

  //*************************************************************************//
  Scene::~Scene()
  {
    //-----------------------------------------------------------------------//
    // Delete all meshes
    for(unsigned long ulIndex = 0UL; ulIndex < this->mvMeshes.size(); ulIndex++)
    {
      delete this->mvMeshes[ulIndex];
    }
    this->mvMeshes.clear();
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    // Delete all lights
    for(unsigned long ulIndex = 0UL; ulIndex < this->mvLights.size(); ulIndex++)
    {
      delete this->mvLights[ulIndex];
    }
    this->mvLights.clear();
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    // Delete all cameras
    for(unsigned long ulIndex = 0UL; ulIndex < this->mvCameras.size(); ulIndex++)
    {
      delete this->mvCameras[ulIndex];
    }
    this->mvCameras.clear();
    this->mpActiveCamera = NULL;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    // Delete all materials
    for(unsigned long ulIndex = 0UL; ulIndex < this->mvMaterials.size(); ulIndex++)
    {
      delete this->mvMaterials[ulIndex];
    }
    this->mvMaterials.clear();
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    // Delete all textures
    for(unsigned long ulIndex = 0UL; ulIndex < this->mvTextures.size(); ulIndex++)
    {
      delete this->mvTextures[ulIndex];
    }
    this->mvTextures.clear();
    //-----------------------------------------------------------------------//
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::Load(std::ifstream& ifs)
  {
    bool bReturnValue = false;
    assert(ifs.is_open());

    if(!ifs.is_open())
      return bReturnValue;

    unsigned long ulLightCount = 0UL;

    while(!ifs.eof())
    {
      string strLine;
      getline(ifs, strLine);
      stringstream sstmLine(strLine);
      string strType;
      sstmLine >> strType;
      if(strType == CAMERA_LABEL)
      {
        // Get Camera Name
        string strName;
        sstmLine >> strName;
        class Camera* pCamera = this->CreateCamera(strName);
        if(!pCamera->Load(ifs))
        {
          this->DestroyCamera(strName);
        }
        bReturnValue = true;
      }
      else if(strType == LIGHT_LABEL)
      {
        // Get Light Name
        string strName;
        sstmLine >> strName;
        enum LightID eID;
        if(ulLightCount == 0)
          eID = LIGHT_ID_0;
        else if(ulLightCount == 1)
          eID = LIGHT_ID_1;
        else if(ulLightCount == 2)
          eID = LIGHT_ID_2;
        else if(ulLightCount == 3)
          eID = LIGHT_ID_3;
        else if(ulLightCount == 4)
          eID = LIGHT_ID_4;
        else if(ulLightCount == 5)
          eID = LIGHT_ID_5;
        else if(ulLightCount == 6)
          eID = LIGHT_ID_6;
        else if(ulLightCount == 7)
          eID = LIGHT_ID_7;
        else 
          eID = LIGHT_ID_COUNT;
        ulLightCount++;
        assert(eID != LIGHT_ID_COUNT);
        if(eID != LIGHT_ID_COUNT)
        {
          class Light* pLight = this->CreateLight(strName, eID);
          if(!pLight->Load(ifs))
          {
            this->DestroyLight(strName);
          }
          bReturnValue = true;
        }
      }
      else if(strType == MATERIAL_LABEL)
      {
        // Get Material Name
        string strName;
        sstmLine >> strName;
        class Material* pMaterial = this->CreateMaterial(strName);
        if(!pMaterial->Load(ifs))
        {
          this->DestroyMaterial(&pMaterial);
          bReturnValue = false;
        }
      }
      else if(strType == MESH_LABEL)
      {
        // Get Mesh Name
        string strName;
        sstmLine >> strName;
        class Mesh* pMesh = this->CreateMesh(strName);
        if(!pMesh->Load(ifs))
        {
          this->DestroyMesh(&pMesh);
        }
        else
        {
          GeometryBuffer* pBuffer = NULL;
          if((this->mvGeometryBuffers.size() == 0) || 
            (this->mvGeometryBuffers.back()->VertexFreeElements() < pMesh->VertexCount()) ||
            (this->mvGeometryBuffers.back()->IndexFreeElements() < pMesh->IndexCount()))
          {
            pBuffer = new GeometryBuffer(this->mpGraphics);
            this->mvGeometryBuffers.push_back(pBuffer);
          }
          else
          {
            pBuffer = this->mvGeometryBuffers.back();
          }
          unsigned long ulVertexOffset = pBuffer->UsedVertexElements();

          if(ulVertexOffset > 0)
          {
            for(unsigned long ulIndex = 0UL; ulIndex < pMesh->IndexCount(); ulIndex++)
            {
              pMesh->IndexArray()[ulIndex] += ulVertexOffset;
            }
          }

          unsigned long ulIndexOffset = pBuffer->Allocate(pMesh->VertexArray(), pMesh->VertexCount(), pMesh->IndexArray(), pMesh->IndexCount());
          if(ulIndexOffset != ULONG_MAX)
            pMesh->AssignBuffer(pBuffer, ulIndexOffset);
        }

      }
      else if(strType == TEXTURE_LABEL)
      {
        string strTextureType;
        string strTextureName;
        sstmLine >> strTextureType;
        sstmLine >> strTextureName;
        enum TextureType eType = NO_TEXTURE_TYPE;
        if(strTextureType == "TGA")
        {
          eType = TGA_TEXTURE_TYPE;
        }

        if(eType != NO_TEXTURE_TYPE)
        {
          class Texture* pTexture = CreateTexture(strTextureName, eType);
          if(!pTexture->Load(ifs))
          {
            this->DestroyTexture(&pTexture);
          }
          else
          {
            pTexture->Initialize();
          }
        }
      }
      else if(strType == SHADER_LABEL)
      {
        string strShaderType;
        string strShaderName;
        sstmLine >> strShaderType;
        sstmLine >> strShaderName;
        
        Shader* pShader = NULL;
        if(strShaderType == "Vertex")
        {
          pShader = this->CreateShader(strShaderName, MST_GRAPHICS_VERTEX_SHADER);
        }
        else if(strShaderType == "Fragment")
        {
          pShader = this->CreateShader(strShaderName, MST_GRAPHICS_FRAGMENT_SHADER);
        }

        if(pShader)
        {
          if(!pShader->Load(ifs))
          {
            this->DestroyShader(&pShader);
          }
          else if(!pShader->Compile())
          {
            this->DestroyShader(&pShader);
          }
        }
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  void Scene::Render()
  {
    this->mpActiveCamera->Render();

    for(unsigned short i = 0; i < mvMeshes.size(); i++)
    {
      mvMeshes[i]->Render();
    }
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::SetupCamera()
  {
    bool bReturnValue = true;
    // Camera constructor will setup a default parameters for the camera. There 
    // is no need to call LoadDefaults again.
    class Camera* pCamera = this->CreateCamera(static_cast<string>("DEFAULT_CAM"));
    this->mvCameras.push_back(pCamera);
    this->mpActiveCamera = pCamera;
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::SetupLight()
  {
    bool bReturnValue = true;

    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Mesh* const Scene::CreateMesh(const std::string& rstrName)
  {
    class Mesh* pReturnValue = NULL;
    if(this->GetMesh(rstrName) == NULL)
    {
      pReturnValue = new class Mesh(rstrName, this);
      this->mvMeshes.push_back(pReturnValue);
    }
    return pReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  Mesh* const Scene::CloneMesh(const std::string& rstrSourceName, const std::string& rstrDestinationName)
  {
    Mesh* pReturnValue = this->GetMesh(rstrSourceName);
    if((pReturnValue != NULL) && (this->GetMesh(rstrDestinationName) == NULL))
    {
      pReturnValue = new class Mesh(*pReturnValue, rstrDestinationName);
      this->mvMeshes.push_back(pReturnValue);
    }
    return pReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyMesh(const std::string& rstrName)
  {
    bool bReturnValue = false;
    vector<class Mesh*>::iterator itr = this->mvMeshes.begin();
    while(itr != this->mvMeshes.end())
    {
      if((*itr)->Name() == rstrName)
      {
        delete *itr;
        this->mvMeshes.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyMesh(class Mesh** ppMesh)
  {
    bool bReturnValue = false;
    vector<class Mesh*>::iterator itr = this->mvMeshes.begin();
    while(itr != this->mvMeshes.end())
    {
      if(*itr == *ppMesh)
      {
        delete *itr;
        this->mvMeshes.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Mesh* const Scene::GetMesh(const std::string& rstrName)
  {
    for(unsigned long i = 0; i < this->mvMeshes.size(); i++)
    {
      if(this->mvMeshes[i]->Name() == rstrName)
        return this->mvMeshes[i];
    }
    return NULL;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Camera* const Scene::CreateCamera(const std::string& rstrName)
  {
    class Camera* pReturnValue = NULL;
    if(this->GetCamera(rstrName) == NULL)
    {
      pReturnValue = new class Camera(rstrName, this);
      this->mvCameras.push_back(pReturnValue);
      this->mpActiveCamera = pReturnValue;
    }
    return pReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyCamera(const std::string& rstrName)
  {
    bool bReturnValue = false;
    vector<class Camera*>::iterator itr = this->mvCameras.begin();
    while(itr != this->mvCameras.end())
    {
      if((*itr)->Name() == rstrName)
      {
        delete *itr;
        this->mvCameras.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyCamera(class Camera** ppCamera)
  {
    bool bReturnValue = false;
    vector<class Camera*>::iterator itr = this->mvCameras.begin();
    while(itr != this->mvCameras.end())
    {
      if(*itr == *ppCamera)
      {
        delete *itr;
        this->mvCameras.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Camera* const Scene::GetCamera(const std::string& rstrName)
  {
    // TODO Make this quicker
    for(unsigned long i = 0; i < this->mvCameras.size(); i++)
    {
      if(this->mvCameras[i]->Name() == rstrName)
        return this->mvCameras[i];
    }
    return NULL;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Camera* const Scene::GetActiveCamera()
  {
    return this->mpActiveCamera;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Light* const Scene::CreateLight(const std::string& rstrName, enum LightID eLightID)
  {
    class Light* pReturnValue = NULL;
    if(this->GetLight(rstrName) == NULL)
    {
      pReturnValue = new class Light(rstrName, eLightID);
      this->mvLights.push_back(pReturnValue);
    }
    return pReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyLight(const std::string& rstrName)
  {
    bool bReturnValue = false;
    vector<class Light*>::iterator itr = this->mvLights.begin();
    while(itr != this->mvLights.end())
    {
      if((*itr)->Name() == rstrName)
      {
        delete *itr;
        this->mvLights.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyLight(class Light** ppLight)
  {
    bool bReturnValue = false;
    vector<class Light*>::iterator itr = this->mvLights.begin();
    while(itr != this->mvLights.end())
    {
      if(*itr == *ppLight)
      {
        delete *itr;
        this->mvLights.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Light* const Scene::GetLight(const std::string& rstrName)
  {
    // TODO Make this quicker
    for(unsigned long i = 0; i < this->mvLights.size(); i++)
    {
      if(this->mvLights[i]->Name() == rstrName)
        return this->mvLights[i];
    }
    return NULL;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Material* const Scene::CreateMaterial(const std::string& rstrName)
  {
    class Material* pReturnValue = NULL;
    if(this->GetLight(rstrName) == NULL)
    {
      pReturnValue = new class Material(rstrName);
      this->mvMaterials.push_back(pReturnValue);
    }
    return pReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyMaterial(const std::string& rstrName)
  {
    bool bReturnValue = false;
    vector<class Material*>::iterator itr = this->mvMaterials.begin();
    while(itr != this->mvMaterials.end())
    {
      if((*itr)->Name() == rstrName)
      {
        delete *itr;
        this->mvMaterials.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyMaterial(class Material** ppMaterial)
  {
    bool bReturnValue = false;
    vector<class Material*>::iterator itr = this->mvMaterials.begin();
    while(itr != this->mvMaterials.end())
    {
      if(*itr == *ppMaterial)
      {
        delete *itr;
        this->mvMaterials.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Material* const Scene::GetMaterial(const std::string& rstrName)
  {
    // TODO Make this quicker
    for(unsigned long i = 0; i < this->mvMaterials.size(); i++)
    {
      if(this->mvMaterials[i]->Name() == rstrName)
        return this->mvMaterials[i];
    }
    return NULL;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Texture* const Scene::CreateTexture(const std::string& rstrName, enum TextureType eTextureType)
  {
    class Texture* pReturnValue = NULL;
    if(this->GetTexture(rstrName) == NULL)
    {
      if(eTextureType == TGA_TEXTURE_TYPE)
        pReturnValue = new TGA(rstrName);
      else if(eTextureType == NO_TEXTURE_TYPE)
        pReturnValue = new Texture(rstrName);

      if(pReturnValue)
        this->mvTextures.push_back(pReturnValue);
    }
    return pReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyTexture(const std::string& rstrName)
  {
    bool bReturnValue = false;
    vector<class Texture*>::iterator itr = this->mvTextures.begin();
    while(itr != this->mvTextures.end())
    {
      if((*itr)->Name() == rstrName)
      {
        delete *itr;
        this->mvTextures.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyTexture(class Texture** ppTexture)
  {
    bool bReturnValue = false;
    vector<class Texture*>::iterator itr = this->mvTextures.begin();
    while(itr != this->mvTextures.end())
    {
      if(*itr == *ppTexture)
      {
        delete *itr;
        this->mvTextures.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Texture* const Scene::GetTexture(const std::string& rstrName)
  {
    for(unsigned long i = 0; i < this->mvTextures.size(); i++)
    {
      if(this->mvTextures[i]->Name() == rstrName)
        return this->mvTextures[i];
    }
    return NULL;
  }
  //-------------------------------------------------------------------------//


  //-------------------------------------------------------------------------//
  class Shader* const Scene::CreateShader(const std::string& rstrName, enum ShaderType eShaderType)
  {
    class Shader* pReturnValue = NULL;
    if(this->GetShader(rstrName) == NULL)
    {
      pReturnValue = new Shader(rstrName, eShaderType);

      if(pReturnValue)
        this->mvShaders.push_back(pReturnValue);
    }
    return pReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyShader(const std::string& rstrName)
  {
    bool bReturnValue = false;
    vector<class Shader*>::iterator itr = this->mvShaders.begin();
    while(itr != this->mvShaders.end())
    {
      if((*itr)->Name() == rstrName)
      {
        delete *itr;
        this->mvShaders.erase(itr);
        bReturnValue = true;
        break;
      }
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  bool Scene::DestroyShader(class Shader** ppShader)
  {
    bool bReturnValue = false;
    vector<class Shader*>::iterator itr = this->mvShaders.begin();
    while(itr != this->mvShaders.end())
    {
      if(*itr == *ppShader)
      {
        delete *itr;
        this->mvShaders.erase(itr);
        bReturnValue = true;
        break;
      }
      else
        itr++;
    }
    return bReturnValue;
  }
  //-------------------------------------------------------------------------//

  //-------------------------------------------------------------------------//
  class Shader* const Scene::GetShader(const std::string& rstrName)
  {
    for(unsigned long i = 0; i < this->mvShaders.size(); i++)
    {
      if(this->mvShaders[i]->Name() == rstrName)
        return this->mvShaders[i];
    }
    return NULL;
  }
  //-------------------------------------------------------------------------//
}
//***************************************************************************//