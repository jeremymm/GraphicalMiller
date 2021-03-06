//***************************************************************************//
// Mesh Class Interface
//
// Created: July 08, 2005
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

#ifndef MST_GRAPHICS_MESH_HPP
#define MST_GRAPHICS_MESH_HPP

//***************************************************************************//
// System includes
#include <string>
#include <vector>
//***************************************************************************//

//***************************************************************************//
// Local Includes
#include "Object.hpp"
//***************************************************************************//

//***************************************************************************//
namespace MST
{
  //*************************************************************************//
  const unsigned char MAX_NUMBER_OF_LODS = 10;
  //*************************************************************************//

  //*************************************************************************//
  class BoundingVolume
  {
  private:
    //-----------------------------------------------------------------------//
    MST::Point <float, 3>  mUpperFrontRight;
    MST::Point <float, 3>  mLowerBackLeft;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    void Create();
    void Clone(const BoundingVolume& rSource);
    void Destroy();
    //-----------------------------------------------------------------------//
  public:
    //-----------------------------------------------------------------------//
    BoundingVolume();
    BoundingVolume(const BoundingVolume& rSource);
    ~BoundingVolume();
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    void Examine(const class Vertex& rVertex);
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    float Width()  const;
    float Height() const;
    float Depth()  const;
    //-----------------------------------------------------------------------//
  };
  //*************************************************************************//

  //*************************************************************************//
  // Mesh class manages polygons that describe the mesh.
  class Mesh : public Object
  {
  private:
    //***********************************************************************//
    //-----------------------------------------------------------------------//
    unsigned long muiShaderProgram;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    class Surface* mpSurface;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    class GeometryBuffer* mpGeometryBuffer;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    bool mbWireFrame;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    BoundingVolume* mpBoundingVolume;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    bool AttachShader(class Shader* pShader);
    void LinkShaders();

    bool mbUsingVertexShaderForAffineTransformations;

    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    void Create(const std::string& rstrName, class Scene* pScene);
    void Clone(const Mesh& rSource);
    void Destroy();
    //-----------------------------------------------------------------------//
    //***********************************************************************//
  protected:
    //***********************************************************************//
    //-----------------------------------------------------------------------//
    class Scene* mpScene;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    unsigned long mulNumberOfFaces;
    unsigned long mulNumberOfVertices;
    unsigned long mulNumberOfIndices;
    unsigned long mulIndicesOffset;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    class Vertex*  mpVertexArray;
    unsigned long* mpIndexArray;
    //-----------------------------------------------------------------------//

    //***********************************************************************//
  public:
    //***********************************************************************//
    //-----------------------------------------------------------------------//
    // This constructor sets all member data to zero
    Mesh(const std::string& rstrName, class Scene* const pScene);
    Mesh(const Mesh& rSource);
    Mesh(const Mesh& rSource, const std::string& rstrDestinationName); 
    virtual ~Mesh();
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    Mesh& Mesh::operator=(const Mesh& rSource)
    {
      if(this != &rSource)
      {
        Clone(rSource);
      }    
      return *this;
    }
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    virtual bool  Load(std::ifstream& ifs);
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    void  Position(float fX, float fY, float fZ);
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    class Vertex* VertexArray();
    unsigned long* IndexArray();
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    unsigned long VertexCount() const;
    unsigned long IndexCount() const;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    void AssignBuffer(class GeometryBuffer* const pBuffer, unsigned long ulOffset);
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    // Renders the mesh
    virtual void  Render();
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    const BoundingVolume* const BoundVolume() { return this->mpBoundingVolume; }
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    void  SetLOD(unsigned long ulLOD);
    void  SetWireframe(bool bWireframe) { mbWireFrame = bWireframe; }
    //***********************************************************************//
  }; // End of class Mesh : public Object
  //*************************************************************************//
} // End of namespace MST
//***************************************************************************//
#endif MST_GRAPHICS_MESH_HPP