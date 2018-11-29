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
    glVertexAttribPointer(_position, sizeof(T) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);
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
protected:
  bool _vaoDirty = true;
  VertexArray _VAO;
  VertexBuffer<glm::vec2> _vertexVBO, _positionVBO;
  VertexBuffer<glm::vec4> _colorVBO;
  VertexBuffer<glm::float32> _rotationVBO;
  VertexBuffer<glm::vec2> _sizeVBO;

public:
  struct Instance {
    glm::vec2 position;
    glm::vec4 color;
    glm::float32 rotation;
    glm::vec2 size;
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

  BaseBatch& rotation(const glm::float32& rot) {
    _vaoDirty = true;
    instances.back().rotation = rot;
    return *this;
  }

  BaseBatch& size(const glm::vec2& size) {
    _vaoDirty = true;
    instances.back().size = size;
    return *this;
  }

  virtual void draw(View& view) = 0;

protected:
  const Shader& _shader = ResourceManager::getShader(SHADER_INDEX::INSTANCED_ARRAY);

  BaseBatch() {
    _VAO.addAttrib(_vertexVBO);
    _VAO.addAttribInstanced(_positionVBO);
    _VAO.addAttribInstanced(_colorVBO);
    _VAO.addAttribInstanced(_rotationVBO);
    _VAO.addAttribInstanced(_sizeVBO);
  }

  void _updateVAO(const std::vector<glm::vec2>& vertices) {
    if (!_vaoDirty) return;

    std::vector<glm::vec2> positions;
    std::vector<glm::vec4> colors;
    std::vector<glm::float32> rotations;
    std::vector<glm::vec2> sizes;
    for (const Instance& i : instances) {
      positions.push_back(i.position);
      colors.push_back(i.color);
      rotations.push_back(i.rotation);
      sizes.push_back(i.size);
    }
    _vertexVBO.setVertices(vertices);
    _positionVBO.setVertices(positions);
    _colorVBO.setVertices(colors);
    _rotationVBO.setVertices(rotations);
    _sizeVBO.setVertices(sizes);
  }

  void _drawVerticesInstanced(View& view, const std::vector<glm::vec2>& vertices, GLenum mode) {
    _updateVAO(vertices);

    _shader.use();
    _shader.setMat4("projection", view.proj());
    glBindVertexArray(_VAO.id);
    glDrawArraysInstanced(mode, 0, _vertexVBO.count(), instances.size());
    glBindVertexArray(0);
  }
};

class RectangleBatch : public BaseBatch {
  std::vector<glm::vec2> _rectangleVertices{{0.5, 0.5}, {0.5, -0.5}, {-0.5, 0.5}, {-0.5, -0.5}};

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

class LineBatch : public RectangleBatch {
  constexpr static float _defaultInstanceWidth = 0.1f;
  float _instanceWidth = _defaultInstanceWidth;  
  void _applyWidth() {
    size({instances.back().size.x, _instanceWidth});
  }

public:
  LineBatch() {}

  LineBatch& add(Instance i) {
    _vaoDirty = true;
    instances.push_back(i);
    _instanceWidth = _defaultInstanceWidth;
    return *this;
  }

  LineBatch& add() {
    return add(Instance());
  }

  LineBatch& position(const glm::vec2& pos) {
    _vaoDirty = true;
    instances.back().position = pos;
    return *this;
  }

  LineBatch& color(const glm::vec4& col) {
    _vaoDirty = true;
    instances.back().color = col;
    return *this;
  }

  LineBatch& rotation(const glm::float32& rot) {
    _vaoDirty = true;
    instances.back().rotation = rot;
    return *this;
  }

  LineBatch& points(const glm::vec2& a, const glm::vec2& b) {
    position((a + b) / 2.f);
    size({glm::distance(a, b), 1.f});
    glm::vec2 dir = glm::normalize(b - a);
    rotation(-atan2(dir.y, dir.x));
    _applyWidth();
    return *this;
  }

  LineBatch& lineWidth(float width) {
    _instanceWidth = width;
    _applyWidth();
    return *this;
  }
};
