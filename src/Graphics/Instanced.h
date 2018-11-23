#pragma once

#include "Shader.h"
#include "View.h"
#include "ResourceManager.h"

#include <array>
#include <sstream>
#include <vector>

class InstancedRectangle {
private:
  static GLuint _VAO;
  static const GLfloat _base_vertices[12];
  Shader& _shader = ResourceManager::getShader(SHADER_INDEX::INSTANCED);

  std::vector<glm::vec2> _positions;
  glm::vec2 _size;
  glm::vec3 _color{1, 1, 1};

  void _create(GLuint* VAO, const float* vertices, std::size_t byte_size) {
    GLuint VBO;
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices, GL_STATIC_DRAW);

    // base position attribute
    glBindVertexArray(*VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
  }

public:
  InstancedRectangle() {
    if (not _VAO) _create(&_VAO, _base_vertices, 12 * sizeof(float));
  }

  InstancedRectangle& positions(const std::vector<glm::vec2>& pos) {
    _positions = pos;
    return *this;
  }

  InstancedRectangle& size(GLfloat w, GLfloat h) {
    _size = glm::vec2(w, h);
    return *this;
  }

  InstancedRectangle& color(GLfloat r, GLfloat g, GLfloat b) {
    _color = glm::vec3(r, g, b);
    return *this;
  }
  InstancedRectangle& color(glm::vec3 c) {
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

    // trasnfer positions
    for (int i = 0; i < 100; ++i) {
      std::stringstream a; a << i;
      auto s = "positions[" + a.str() + "]";
      _shader.setVec2(s, _positions[i]);
    }

    glBindVertexArray(_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _positions.size());
    glBindVertexArray(0);
  }
};