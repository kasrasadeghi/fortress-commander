#pragma once

// from learnopengl.com

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  /// https://www.khronos.org/opengl/wiki/OpenGL_Object#Object_zero
  unsigned int ID = 0; // the 0 opengl object is null for shaders
  std::string _name;

  Shader(std::string name) : _name(name){};

  void load() {
    load("shaders/" + _name + ".vs", "shaders/" + _name + ".fs");
  }
  void load(std::string vertexPath, std::string fragmentPath, std::string geometryPath = "");
  // activate the shader
  void use() const {
    glUseProgram(ID);
  }

  // utility uniform functions
  void setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  void setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }
  void setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }
  void setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string& name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }
  void setMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  void setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  void setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

private:
  void checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- "
                  << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- "
                  << std::endl;
      }
    }
  }
};