#pragma once

#include "Shader.h"
#include "View.h"
#include "ResourceManager.h"

#include <array>
#include <sstream>
#include <vector>

inline GLuint createVertexBuffer(const std::vector<glm::vec2>& vertices) {
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VBO;
}

class VertexArray {
public:
  static GLuint create(GLuint* vertVBO, GLuint* posVBO, const std::vector<glm::vec2>& vertices,
                      const std::vector<glm::vec2>& pos) {

    *posVBO = createVertexBuffer(pos);
    *vertVBO = createVertexBuffer(vertices);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, *vertVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

      glBindBuffer(GL_ARRAY_BUFFER, *posVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.
      glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    return VAO;
  }
};

class InstancedArrayRectangle {
  GLuint _VAO = 0;
  GLuint _VBO = 0;
  GLuint _instanceVBO = 0;
  Shader& _shader = ResourceManager::getShader(SHADER_INDEX::INSTANCED_ARRAY);

  glm::vec2 _size;
  glm::vec3 _color{1, 1, 1};
  const uint _position_count;

public:
  static constexpr GLuint batch_size = 5000;

  InstancedArrayRectangle(const std::vector<glm::vec2>& pos): _position_count(pos.size()) {
    std::vector<glm::vec2> base_vertices;
    
    base_vertices.emplace_back(1.f, 1.f);
    base_vertices.emplace_back(1.f, 0.f);
    base_vertices.emplace_back(0.f, 1.f);
    base_vertices.emplace_back(0.f, 0.f);
    
    _VAO = VertexArray::create(&_VBO, &_instanceVBO, base_vertices, pos);
  }

  ~InstancedArrayRectangle() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_instanceVBO);
  }

  InstancedArrayRectangle& size(GLfloat w, GLfloat h) {
    _size = glm::vec2(w, h);
    return *this;
  }

  InstancedArrayRectangle& color(GLfloat r, GLfloat g, GLfloat b) {
    _color = glm::vec3(r, g, b);
    return *this;
  }

  InstancedArrayRectangle& color(glm::vec3 c) {
    _color = c;
    return *this;
  }

  const glm::vec2& size() { return _size; }

  void draw(View& view) {
    _shader.use();
    _shader.setMat4("projection", view.proj());
    _shader.setVec2("size", size());
    _shader.setVec3("color", _color);

    glBindVertexArray(_VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 8, _position_count);
    glBindVertexArray(0);
  }
};


class InstancedCircle {
  GLuint _VAO = 0;
  GLuint _VBO = 0;
  GLuint _instanceVBO = 0;
  Shader& _shader = ResourceManager::getShader(SHADER_INDEX::INSTANCED_ARRAY);

  glm::vec2 _size;
  glm::vec3 _color{1, 1, 1};
  const uint _position_count;

  uint _size_count;

public:
  static constexpr GLuint batch_size = 5000;

  InstancedCircle(const std::vector<glm::vec2>& pos, uint size_count = 30): _position_count(pos.size()), _size_count(size_count) {
    std::vector<glm::vec2> base_vertices;

    base_vertices.emplace_back(0, 0);
    
    for (uint i = 0; i < size_count; ++i) {

      float angle = i * 2 * glm::pi<float>() / size_count - glm::half_pi<float>();
      float x = std::sin(angle) * 0.49;
      float y = std::cos(angle) * 0.49;
      base_vertices.emplace_back(x, y);
    }

    base_vertices.push_back(base_vertices[1]);

    _VAO = VertexArray::create(&_VBO, &_instanceVBO, base_vertices, pos);
  }

  ~InstancedCircle() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_instanceVBO);
  }

  InstancedCircle& size(GLfloat w, GLfloat h) {
    _size = glm::vec2(w, h);
    return *this;
  }

  InstancedCircle& color(GLfloat r, GLfloat g, GLfloat b) {
    _color = glm::vec3(r, g, b);
    return *this;
  }

  InstancedCircle& color(glm::vec3 c) {
    _color = c;
    return *this;
  }

  const glm::vec2& size() { return _size; }

  void draw(View& view) {
    _shader.use();
    _shader.setMat4("projection", view.proj());
    _shader.setVec2("size", size());
    _shader.setVec3("color", _color);

    glBindVertexArray(_VAO);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, _size_count + 2, _position_count);
    glBindVertexArray(0);
  }
};