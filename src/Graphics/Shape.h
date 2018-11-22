#pragma once

#include "Shader.h"
#include "View.h"
#include "ResourceManager.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>

class Shape {
protected:
  Shader& _shader = ResourceManager::getShader(SHADER_INDEX::TRIANGLE);

  glm::vec2 _position;
  glm::vec2 _size;
  GLfloat   _rotate = 0;
  glm::vec3 _color{1, 1, 1};
  glm::mat4 _model;

  ///_create should be called once per _base_vertices to assign a child's static VAO
  void _create(GLuint* VAO, const float* vertices, std::size_t byte_size) {
    GLuint VBO;
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices, GL_STATIC_DRAW);

    // position attribute
    glBindVertexArray(*VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
  }
public:

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

  Shape& color(GLfloat r, GLfloat g, GLfloat b) {
    _color = glm::vec3(r, g, b);
    return *this;
  }
  Shape& color(glm::vec3 c) {
    _color = c;
    return *this;
  }

  const glm::vec2& position() { return _position; }
  const glm::vec2& size() { return _size; }
  const GLfloat& rotate() { return _rotate; }
  const glm::mat4& model() { return _model; }

  const glm::mat4& computeModel() {
    glm::mat4 m(1);
    m = glm::translate(m, glm::vec3(_position, 0.f));

    m = glm::translate(m, glm::vec3(_position * .5f, 0.f));
    m = glm::rotate(m, _rotate, glm::vec3(0.f, 0.f, 1.f));
    m = glm::translate(m, glm::vec3(_position * -.5f, 0.f));

    m = glm::scale(m, glm::vec3(_size, 1.f));
    _model = m;
    return _model;
  }

  virtual void draw(View& view) = 0;

  virtual ~Shape() {};
};

class RectangleShape : public Shape {
  static GLuint _VAO;
  static const std::array<float, 8> _base_vertices;

public:
  RectangleShape() {
    if (not _VAO) _create(&_VAO, _base_vertices.data(), _base_vertices.size() * sizeof(float));
  }
  
  ~RectangleShape() {};

  void draw(View& view) override {
    _shader.use();
    if (view.dirty()) _shader.setMat4("projection", view.proj());
    _shader.setMat4("model", computeModel());
    _shader.setVec3("color", _color);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
  }
};
