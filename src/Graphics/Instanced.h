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

class InstancedArrayRectangle {
  GLuint _VAO = 0;
  GLuint _VBO = 0;
  GLuint _instanceVBO = 0;
  std::vector<glm::vec2> _base_vertices;
  Shader& _shader = ResourceManager::getShader(SHADER_INDEX::INSTANCED_ARRAY);

  const std::vector<glm::vec2>& _positions;
  glm::vec2 _size;
  glm::vec3 _color{1, 1, 1};

  static void _create(GLuint* VAO, GLuint* vertVBO, GLuint* posVBO, const std::vector<glm::vec2>& vertices,
                      const std::vector<glm::vec2>& pos) {

    *posVBO = createVertexBuffer(pos);
    *vertVBO = createVertexBuffer(vertices);

    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);
      // base position attribute
      glBindBuffer(GL_ARRAY_BUFFER, *vertVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

      glBindBuffer(GL_ARRAY_BUFFER, *posVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.

    glBindVertexArray(0);
  }

public:
  static constexpr GLuint batch_size = 5000;

  InstancedArrayRectangle(const std::vector<glm::vec2>& pos): _positions(pos) {
    _base_vertices.emplace_back(1.f, 1.f);
    _base_vertices.emplace_back(1.f, 0.f);
    _base_vertices.emplace_back(0.f, 1.f);
    _base_vertices.emplace_back(0.f, 0.f);
    
    _create(&_VAO, &_VBO, &_instanceVBO, _base_vertices, _positions);
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

  const glm::vec2& positions(int i) { return _positions[i]; }
  const glm::vec2& size() { return _size; }

  void draw(View& view) {
    _shader.use();
    _shader.setMat4("projection", view.proj());
    _shader.setVec2("size", size());
    _shader.setVec3("color", _color);

    glBindVertexArray(_VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 8, _positions.size());
    glBindVertexArray(0);
  }
};