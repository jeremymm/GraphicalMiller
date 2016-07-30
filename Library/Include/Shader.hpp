//***************************************************************************//
// Shader Class Interface
//
// Created: April 11, 2011
// By: Jeremy Michael Miller
//
// Copyright (c) 2011-2016 Jeremy Michael Miller. 
// Author: = "Jeremy Michael Miller"
// Copyright: = "Copyright 2005-2016, Graphical Miller,  All rights reserved."
// Credits = ["Jeremy Michael Miller"]
// License: "Fair use v0.9"
// Version: "0.0.1"
// Maintainer: "Jeremy Michael Miller"
// Email: "maybe_later@mst.dnsalias.net"
// Status: "Alpha"
//***************************************************************************//

#ifndef MST_GRAPHICS_SHADER_HPP
#define MST_GRAPHICS_SHADER_HPP

//***************************************************************************//
// 3rd Party Includes
#include <glew.h> // Must be called before GL.h
//***************************************************************************//

//***************************************************************************//
// Platform Includes
#include <Windows.h>
#include <GL\GL.h>
//***************************************************************************//

//***************************************************************************//
// System Includes
#include <string>
#include <map>
//***************************************************************************//

//***************************************************************************//
namespace MST
{
  //*************************************************************************//
  enum ShaderType
  {
    MST_GRAPHICS_VERTEX_SHADER   = GL_VERTEX_SHADER_ARB,
    MST_GRAPHICS_FRAGMENT_SHADER = GL_FRAGMENT_SHADER_ARB,
    MST_GRAPHICS_UNKNOWN_SHADER  = 0xFFFFFFFF
  };
  //*************************************************************************//

  //*************************************************************************//
  class Shader
  {
  private:
    //***********************************************************************//
    std::string mstrName;
    std::string mstrFilename;

    unsigned int muiShaderID;

    enum ShaderType meType;

    std::map<std::string, unsigned int> mmapParametersToLocation;
    //***********************************************************************//
  public:
    //***********************************************************************//
    //-----------------------------------------------------------------------//
    Shader(const std::string& rstrName, enum ShaderType eShaderType);
    ~Shader();
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    const std::string& Name() const;
    unsigned int ID() const;
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    bool  Load(std::ifstream& ifs);
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    bool  Compile();
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    unsigned int ParameterLocation(const std::string& rstrParameterName);
    //-----------------------------------------------------------------------//

    //-----------------------------------------------------------------------//
    bool UpdateParameter(const std::string& rstrParameterName, float fValue);
    void UpdateParemeter(unsigned int uiLocation, float fValue);
    //-----------------------------------------------------------------------//
    //***********************************************************************//
  };
  //*************************************************************************//
}
//***************************************************************************//

#endif MST_GRAPHICS_SHADER_HPP
