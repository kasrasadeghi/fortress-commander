#pragma once

#include "Instanced.h"
#include "TexturedRectangle.h"
#include "GL.h"

class LightBatch {
protected:
  bool _vaoDirty = true;
  VertexArray _VAO;
  VertexBuffer<glm::vec2> _vertexVBO;
  GL::RenderTexture _renderTex;

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

  void draw(View& view, const RenderWindow& window) {
    // light addition pass
    _renderTex.updateTextureDimensions(window.width(), window.height());
    _renderTex.bindFramebuffer();

    glEnable(GL_BLEND);

    _lightShader.use();
    _lightShader.setMat4("projection", view.proj());
    _update(view);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_ONE, GL_ONE); // additive blending 
    glBindVertexArray(_VAO.id);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _vertexVBO.count());
    glBindVertexArray(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    _renderTex.unbindFramebuffer();

    // multiply to screen
    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ZERO); // color multiply blending
    _renderTex.bindTexture();
    TexturedRectangle(view.topLeft(), view.bottomRight()).draw(view);
    _renderTex.unbindTexture();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
  }

protected:
  const Shader& _lightShader = ResourceManager::getShader(SHADER_INDEX::LIGHT_MASK);

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
    _lightShader.setVec2("positions", positions);
    _lightShader.setVec4("colors", colors);
    _lightShader.setFloat("intensities", intensities);
    _lightShader.setInt("num_lights", instances.size());
  }
};
