#include "Shader.h"
#include "ResourceManager.h"

#include <cerrno>
#include <iostream>

void Shader::load(std::string vertexPath, std::string fragmentPath, std::string geometryPath) {
  // 1. retrieve the vertex/fragment source code from filePath
  std::string vertexCode = ResourceManager::readFile(vertexPath);
  std::string fragmentCode = ResourceManager::readFile(fragmentPath);
  ;
  std::string geometryCode = ResourceManager::readFile(geometryPath);
  if (vertexCode == "" || fragmentCode == "") {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    throw(errno);
  }
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
  // 2. compile shaders
  unsigned int vertex, fragment;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");
  // if geometry shader is given, compile geometry shader
  unsigned int geometry = 0;
  if (geometryPath != "") {
    const char* gShaderCode = geometryCode.c_str();
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    checkCompileErrors(geometry, "GEOMETRY");
  }
  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  if (geometryPath != "") glAttachShader(ID, geometry);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");
  // delete the shaders as they're linked into our program now and no longer
  // necessery
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (geometryPath != "") glDeleteShader(geometry);
}