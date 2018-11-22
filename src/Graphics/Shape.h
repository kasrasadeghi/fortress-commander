#pragma once

#include "Shader.h"
#include "View.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>

class Shape {
protected:
  const Shader _shader {"shaders/view_triangle.vs", "shaders/view_triangle.fs"};

  glm::vec2 _position;
  glm::vec2 _size;
  GLfloat   _rotate = 0;
  glm::mat4 _model;

  void _create(const float* vertices, std::size_t byte_size) {
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  }

public:
  GLuint VAO; // TODO: should this be public?

  Shape& position(GLfloat x, GLfloat y) {
    _position = glm::vec2(x, y);
    return *this;
  }

  Shape& size(GLfloat w, GLfloat h) {
    _size = glm::vec2(w, h);
    return *this;
  }

  Shape& rotate(GLfloat r) {
    _rotate = r;
    return *this;
  }

  const glm::vec2& position() { return _position; }
  const glm::vec2& size() { return _size; }
  const GLfloat& rotate() { return _rotate; }
  const glm::mat4& model() { return _model; }
  const Shader& shader() { return _shader; }

  const glm::mat4& computeModel() {
    glm::mat4 m(1);
    m = glm::translate(m, glm::vec3(_position, 0.f));

    m = glm::translate(m, glm::vec3(_position[0] *  .5f, _position[1] *  .5f, 0.f));
    m = glm::rotate(m, _rotate, glm::vec3(0.f, 0.f, 1.f));
    m = glm::translate(m, glm::vec3(_position[0] * -.5f, _position[1] * -.5f, 0.f));

    m = glm::scale(m, glm::vec3(_size, 1.f));
    _model = m;
    return _model;
  }

  virtual void draw(View view) = 0;
};

class RectangleShape : public Shape {

public:
  static const std::array<float, 8> _base_vertices;

  RectangleShape() {
    _create(_base_vertices.data(), _base_vertices.size() * sizeof(float));
  }

  void draw(View view) override {
    _shader.use();
    _shader.setMat4("projection", view.proj());
    _shader.setMat4("model", computeModel());
    _shader.setVec3("color", 1.f, .7f, 0.f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
  }
};

////////////////// DEPRECATED ///////////////////////////
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