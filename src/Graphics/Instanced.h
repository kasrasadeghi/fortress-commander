#pragma once

#include "Shader.h"
#include "View.h"
#include "ResourceManager.h"

#include <array>
#include <sstream>
#include <vector>

template <typename T>
inline GLuint createVertexBuffer(const std::vector<T>& vertices) {
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VBO;
}

class VertexArray {
public:
  GLuint VAO;
  GLuint vertexVBO, positionVBO, colorVBO;
  const std::size_t count, instanceCount;

  /**
   * @param[out] vertexVBO
   * @param[out] positionVBO
   * @param[out] colorVBO
   * @param[in] vertices
   * @param[in] positions
   * @param[in] colors
   * @return the vertex array object
   */
  VertexArray(const std::vector<glm::vec2>& vertices, const std::vector<glm::vec2>& positions,
              const std::vector<glm::vec4>& colors) : 
              count(vertices.size() + positions.size() + colors.size()),
              instanceCount(positions.size()) {

    vertexVBO = createVertexBuffer(vertices);
    positionVBO = createVertexBuffer(positions);
    colorVBO = createVertexBuffer(colors);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

      glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glVertexAttribDivisor(1, 1); // tell OpenGL there is one position per instance 

      glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glVertexAttribDivisor(2, 1);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
  }

  ~VertexArray() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &positionVBO);
    glDeleteBuffers(1, &colorVBO);
  }
};

class BaseBatch {
public:
  struct Instance {
    glm::vec2 position;
    glm::vec4 color;
  };

  std::vector<Instance> instances;

  BaseBatch& add(Instance i) {
    instances.push_back(i);
    return *this;
  }

  BaseBatch& add() {
    return add(Instance());
  }

  BaseBatch& position(float x, float y) {
    instances.back().position = {x, y};
    return *this; 
  }
    
  BaseBatch& color(float r, float g, float b, float a = 1.f) {
    instances.back().color = {r, g, b, a};
    return *this;
  }
    
  BaseBatch& size(float x, float y) {
    _size = {x, y};
    return *this;
  }

  virtual void draw(View& view) = 0;

protected:
  const Shader& _shader = ResourceManager::getShader(SHADER_INDEX::INSTANCED_ARRAY);
  glm::vec2 _size{1.f, 1.f};

  BaseBatch() {}

  void _drawVerticesInstanced(View& view, const std::vector<glm::vec2>& vertices, GLenum mode) {
    std::vector<glm::vec2> positions;
    std::vector<glm::vec4> colors;
    for (const Instance& i : instances) {
      positions.push_back(i.position);
      colors.push_back(i.color);
    }
    auto va = VertexArray(vertices, positions, colors);

    _shader.use();
    _shader.setMat4("projection", view.proj());
    _shader.setVec2("size", _size); // TODO: instanced size
    glBindVertexArray(va.VAO);
    glDrawArraysInstanced(mode, 0, va.count, va.instanceCount);
    glBindVertexArray(0);
  }
};

class RectangleBatch : public BaseBatch {
  std::vector<glm::vec2> _rectangleVertices{
    {1.f, 1.f},
    {1.f, 0.f},
    {0.f, 1.f},
    {0.f, 0.f}
  };

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
  InstancedRectangle(float x, float y): InstancedRectangle(std::vector{glm::vec2(x, y)}) {}

  InstancedRectangle(const std::vector<glm::vec2>& pos): _pos(pos), _position_count(pos.size()) {}

  InstancedRectangle& size(GLfloat w, GLfloat h) {
    _size = glm::vec2(w, h);
    return *this;
  }

  InstancedRectangle& color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f) {
    _color = glm::vec4(r, g, b, a);
    return *this;
  }

  InstancedRectangle& color(glm::vec4 c) {
    _color = c;
    return *this;
  }

  const glm::vec2& size() { return _size; }

  void draw(View& view) {
    RectangleBatch rectangles;
    rectangles.size(_size.x, _size.y);

    for (glm::vec2 pos : _pos) {
      rectangles.add()
        .position(pos.x, pos.y)
        .color(_color.r, _color.g, _color.b, _color.a);
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
  InstancedCircle(float x, float y): InstancedCircle(std::vector{glm::vec2(x, y)}) {}

  InstancedCircle(const std::vector<glm::vec2>& pos, uint size_count = 30): _pos(pos), _position_count(pos.size()), _size_count(size_count) {}

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

  const glm::vec2& size() { return _size; }

  void draw(View& view) {
    CircleBatch circles;
    circles.size(_size.x, _size.y);

    for (glm::vec2 pos : _pos) {
      circles.add()
        .position(pos.x, pos.y)
        .color(_color.r, _color.g, _color.b, _color.a);
    }

    circles.draw(view); 
  }
};