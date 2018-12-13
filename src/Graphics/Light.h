#pragma once

#include "Instanced.h"
#include "Texture.h"
#include "TexturedRectangle.h"
#include "GL.h"

class LightBatch {
protected:
  bool _vaoDirty = true;
  GL::VertexArray _VAO;
  GL::RenderTexture _renderTex;

public:
  struct Instance {
    glm::vec2 position;
    glm::vec4 color;
    float intensity;
  };

  std::vector<Instance> instances;

  LightBatch() {
    _VAO.setAttributeLayout({2});
    _VAO.setAttributeLayout({2, 4, 1}, true);
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

    _maskShader.use();
    _maskShader.setMat4("projection", view.proj());
    _update(view);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR); // screen blending 
    _VAO.bind();
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instances.size());
    _VAO.unbind();
    _renderTex.unbindFramebuffer();

    // multiply to screen
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha blending

    // multitexture setup
    _cloudShader.use();
    _cloudShader.setInt("bufferTexture", 0);
    _cloudShader.setInt("cloudTexture", 1);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    _renderTex.bindTexture();
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    _cloudTex.bind();
    TexturedRectangle(view.topLeft(), view.bottomRight(), _cloudShader).draw(view);
    _cloudTex.unbind();
    glActiveTexture(GL_TEXTURE0);
    _renderTex.unbindTexture();
  }

protected:
  Texture _cloudTex{"textures/clouds.png", true};
  Shader& _maskShader = ResourceManager::getShader(SHADER_INDEX::FOW_MASK);
  Shader& _cloudShader = ResourceManager::getShader(SHADER_INDEX::FOW_CLOUDS);

  void _update(View& view) {
    if (!_vaoDirty) return;

    _VAO.VB.bindData(std::vector<glm::vec2>{
      {view.left(), view.top()},
      {view.right(), view.top()},
      {view.right(), view.bottom()},
      {view.left(), view.bottom()}
    });

    _VAO.IB.bindData(instances);
  }
};
