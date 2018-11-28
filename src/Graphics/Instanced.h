#pragma once

#include "ResourceManager.h"
#include "Shader.h"
#include "View.h"

#include <array>
#include <memory>
#include <sstream>
#include <vector>

template <typename T>
class VertexBuffer {
  size_t _count;

public:
  GLuint id;

  VertexBuffer() {
    glGenBuffers(1, &id);
  }

  VertexBuffer(const std::vector<T>& vertices) : VertexBuffer() {
    setVertices(vertices);
  }

  void setVertices(const std::vector<T>& vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    _count = vertices.size();
  }

  size_t count() {
    return _count;
  }

  ~VertexBuffer() {
    glDeleteBuffers(1, &id);
  }
};

class VertexArray {
  int _position = 0;

public:
  GLuint id;

  VertexArray() {
    glGenVertexArrays(1, &id);
  }

  ~VertexArray() {
    glDeleteVertexArrays(1, &id);
  }

  template <typename T>
  VertexArray& addAttrib(const VertexBuffer<T>& VBO, bool instanced = false) {
    glBindVertexArray(id);

    glBindBuffer(GL_ARRAY_BUFFER, VBO.id);
    glEnableVertexAttribArray(_position);
    glVertexAttribPointer(_position, T::length(), GL_FLOAT, GL_FALSE, T::length() * sizeof(float), (void*)0);
    if (instanced) glVertexAttribDivisor(_position, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    _position += 1;
    return *this;
  }

  template <typename T>
  VertexArray& addAttribInstanced(const VertexBuffer<T>& VBO) {
    return addAttrib(VBO, true);
  }
};

class BaseBatch {
  bool _vaoDirty = true;
  VertexArray _VAO;
  VertexBuffer<glm::vec2> _vertexVBO, _positionVBO;
  VertexBuffer<glm::vec4> _colorVBO;

public:
  struct Instance {
    glm::vec2 position;
    glm::vec4 color;
  };

  std::vector<Instance> instances;

  BaseBatch& add(Instance i) {
    _vaoDirty = true;
    instances.push_back(i);
    return *this;
  }

  BaseBatch& add() {
    return add(Instance());
  }

  BaseBatch& position(const glm::vec2& pos) {
    _vaoDirty = true;
    instances.back().position = pos;
    return *this;
  }

  BaseBatch& color(const glm::vec4& col) {
    _vaoDirty = true;
    instances.back().color = col;
    return *this;
  }

  BaseBatch& size(const glm::vec2& size) {
    _vaoDirty = true;
    _size = size;
    return *this;
  }

  virtual void draw(View& view) = 0;

protected:
  const Shader& _shader = ResourceManager::getShader(SHADER_INDEX::INSTANCED_ARRAY);
  glm::vec2 _size{1.f, 1.f};

  BaseBatch() {
    _VAO.addAttrib(_vertexVBO);
    _VAO.addAttribInstanced(_positionVBO);
    _VAO.addAttribInstanced(_colorVBO);
  }

  void _updateVAO(const std::vector<glm::vec2>& vertices) {
    if (!_vaoDirty) return;

    std::vector<glm::vec2> positions;
    std::vector<glm::vec4> colors;
    for (const Instance& i : instances) {
      positions.push_back(i.position);
      colors.push_back(i.color);
    }
    _vertexVBO.setVertices(vertices);
    _positionVBO.setVertices(positions);
    _colorVBO.setVertices(colors);
  }

  void _drawVerticesInstanced(View& view, const std::vector<glm::vec2>& vertices, GLenum mode) {
    _updateVAO(vertices);

    _shader.use();
    _shader.setMat4("projection", view.proj());
    _shader.setVec2("size", _size); // TODO: instanced size
    glBindVertexArray(_VAO.id);
    glDrawArraysInstanced(mode, 0, _vertexVBO.count(), instances.size());
    glBindVertexArray(0);
  }
};

class RectangleBatch : public BaseBatch {
  std::vector<glm::vec2> _rectangleVertices{{1.f, 1.f}, {1.f, 0.f}, {0.f, 1.f}, {0.f, 0.f}};

public:
  RectangleBatch() {}

  void draw(View& view) override {
    _drawVerticesInstanced(view, _rectangleVertices, GL_TRIANGLE_STRIP);
  }
};

class CircleBatch : public BaseBatch {
  std::vector<glm::vec2> _circleVertices;

public:
  CircleBatch(uint size_count = 30) {
    _circleVertices.emplace_back(0, 0);

    for (uint i = 0; i < size_count; ++i) {
      float angle = i * 2 * glm::pi<float>() / size_count - glm::half_pi<float>();
      float x = std::sin(angle) * 0.49;
      float y = std::cos(angle) * 0.49;
      _circleVertices.emplace_back(x, y);
    }

    _circleVertices.push_back(_circleVertices[1]);
  }

  void draw(View& view) override {
    _drawVerticesInstanced(view, _circleVertices, GL_TRIANGLE_FAN);
  }
};

class InstancedRectangle {
  std::vector<glm::vec2> _pos;

  glm::vec2 _size;
  glm::vec4 _color{1, 1, 1, 1};
  const uint _position_count;

public:
  InstancedRectangle(float x, float y) : InstancedRectangle(std::vector{glm::vec2(x, y)}) {}

  InstancedRectangle(const std::vector<glm::vec2>& pos) : _pos(pos), _position_count(pos.size()) {}

  InstancedRectangle& size(GLfloat w, GLfloat h) {
    _size = glm::vec2({w, h});
    return *this;
  }

  InstancedRectangle& color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f) {
    _color = glm::vec4({r, g, b, a});
    return *this;
  }

  InstancedRectangle& color(glm::vec4 c) {
    _color = c;
    return *this;
  }

  const glm::vec2& size() {
    return _size;
  }

  void draw(View& view) {
    RectangleBatch rectangles;
    rectangles.size(_size);

    for (glm::vec2 pos : _pos) {
      rectangles.add().position(pos).color(_color);
    }

    rectangles.draw(view);
  }
};

class InstancedCircle {
  std::vector<glm::vec2> _pos;

  glm::vec2 _size;
  glm::vec4 _color{1, 1, 1, 1};
  const uint _position_count;

  uint _size_count;

public:
  InstancedCircle(float x, float y) : InstancedCircle(std::vector{glm::vec2(x, y)}) {}

  InstancedCircle(const std::vector<glm::vec2>& pos, uint size_count = 30)
      : _pos(pos), _position_count(pos.size()), _size_count(size_count) {}

  InstancedCircle& size(GLfloat w, GLfloat h) {
    _size = glm::vec2(w, h);
    return *this;
  }

  InstancedCircle& color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f) {
    _color = glm::vec4(r, g, b, a);
    return *this;
  }

  InstancedCircle& color(glm::vec4 c) {
    _color = c;
    return *this;
  }

  const glm::vec2& size() {
    return _size;
  }

  void draw(View& view) {
    CircleBatch circles;
    circles.size(_size);

    for (glm::vec2 pos : _pos) {
      circles.add().position(pos).color(_color);
    }

    circles.draw(view);
  }
};