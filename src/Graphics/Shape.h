#pragma once

#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>

class Shape {
protected:
  glm::vec2 _position;
  glm::vec2 _size;
  GLfloat   _float;

  glm::mat4 getModel() {
    glm::mat4 m(1);
    m = glm::translate(m, glm::vec3(_position, 0.f));

    m = glm::translate(m, glm::vec3(_position[0] *  .5f, _position[1] *  .5f, 0.f));
    m = glm::rotate(m, _float, glm::vec3(0.f, 0.f, 1.f));
    m = glm::translate(m, glm::vec3(_position[0] * -.5f, _position[1] * -.5f, 0.f));

    m = glm::scale(m, glm::vec3(_size, 1.f));
    return m;
  }
};

class TricolorTriangleShape {
  std::array<float, 18> _vertices;
  const Shader _tricolor_shader{"shaders/tricolor_triangle.vs", "shaders/tricolor_triangle.fs"};

  void _create() {
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(),
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }

public:
  GLuint VAO;

  TricolorTriangleShape(std::array<float, 18> vertices) : _vertices(vertices) { _create(); }

  void draw() {
    _tricolor_shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
};

class UnicolorTriangleShape {
  std::array<float, 6> _vertices;
  glm::vec3 _color{0.f, 0.f, 0.f};
  Shader _unicolor_shader{"shaders/unicolor_triangle.vs", "shaders/unicolor_triangle.fs"};

  void _create() {
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(),
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  }

public:
  GLuint VAO;

  UnicolorTriangleShape(std::array<float, 6> vertices) : _vertices(vertices) { _create(); }

  void setColor(GLfloat r, GLfloat g, GLfloat b) {
    _color = glm::vec3(r, g, b);
  }

  void draw() {
    _unicolor_shader.use();
    _unicolor_shader.setVec3("color", _color);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
};

class RectangleShape {
  Shader shader {"shaders/view_triangle.vs", "shaders/view_triangle.fs"};

public:
  static const std::array<float, 8> _base_vertices;

  void _create() {
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _base_vertices.size() * sizeof(float), _base_vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  }

  GLuint VAO;
};