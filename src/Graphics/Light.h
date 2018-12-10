#pragma once

#include "Instanced.h"

class LightBatch {
protected:
  bool _vaoDirty = true;
  VertexArray _VAO;
  VertexBuffer<glm::vec2> _vertexVBO;

public:
  struct Instance {
    glm::vec2 position;
    glm::vec4 color;
    float intensity;
  };

  std::vector<Instance> instances;

  LightBatch() {
    _VAO.addAttrib(_vertexVBO);
  }

  LightBatch& add(Instance i) {
    _vaoDirty = true;
    instances.push_back(i);
    return *this;
  }

  LightBatch& add() {
    return add(Instance());
  }

  void clear() {
    instances.clear();
  }

  LightBatch& position(const glm::vec2& pos) {
    _vaoDirty = true;
    instances.back().position = pos;
    return *this;
  }

  LightBatch& color(const glm::vec4& col) {
    _vaoDirty = true;
    instances.back().color = col;
    return *this;
  }

  LightBatch& intensity(const glm::float32& intensity) {
    _vaoDirty = true;
    instances.back().intensity = intensity;
    return *this;
  }

  void draw(View& view) {
    _shader.use();
    _update(view);

    _shader.setMat4("projection", view.proj());
    glBlendFunc(GL_DST_COLOR, GL_ZERO);
    glBindVertexArray(_VAO.id);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, _vertexVBO.count(), instances.size());
    glBindVertexArray(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

protected:
  const Shader& _shader = ResourceManager::getShader(SHADER_INDEX::LIGHT_MASK);

  void _update(View& view) {
    if (!_vaoDirty) return;

    _vertexVBO.setVertices(std::vector<glm::vec2>{
      {view.left(), view.top()},
      {view.right(), view.top()},
      {view.right(), view.bottom()},
      {view.left(), view.bottom()}
    });

    // update uniforms
    std::vector<glm::vec2> positions(1024, glm::vec2(0, 0));
    std::vector<glm::vec4> colors(1024, glm::vec4(0, 0, 0, 0));
    std::vector<float> intensities(1024, 0);
    uint index = 0;
    for (const Instance& i : instances) {
      positions[index] = i.position;
      colors[index] = i.color;
      intensities[index] = i.intensity;
      ++index;
    }
    _shader.setVec2("positions", positions);
    _shader.setVec4("colors", colors);
    _shader.setFloat("intensities", intensities);
  }
};
