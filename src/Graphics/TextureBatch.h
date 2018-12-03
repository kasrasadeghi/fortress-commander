#include <glad/glad.h>

#include "Texture.h"

/**
 * Usage example:
 * 
 * <code>
 * Texture t;
 * TextureBatch b {t};
 * auto view = View().center(10,10).radius(10,10);
 * b.view(view);
 * 
 * ...
 * 
 * // preadd and update
 * b.add({.pos = {15, 15}, .size = {2, 2}});
 * b.update();
 * 
 * while (looping) {
 *   ... clear screen ...
 * 
 *   if (need_add) {
 *     b.add({.pos = {10,10}, .size = {3, 3}});
 *     b.update();
 *   } else if (need_reset) {
 *     b.clear();
 *     b.add({.pos = {5,5}, .size = {3, 3}});
 *     b.update();
 *   }
 *   
 *   b.draw();
 *  
 *   ... swap buffers and poll ...
 * }
 * </code>
 * 
 * Full instance add:
 * <code>
 * b.add({
 *   .pos = {x, y},
 *   .size = {w, h},
 *   .color = {r, g, b, a},
 *   .texOffset = {5}, // 0, 1, 2, .. [total # textures in atlas].  offset is divided for you
 *   .rotation = {theta_radians},
 * })
 * </code>
 * 
 */

struct TextureBatch {
  struct Instance {
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec4 aColor = {1, 1, 1, 1};
    float texOffset = 0;
    float rotation = 0;
  };

  View* _view = nullptr; //TODO: delete
  std::vector<Instance> instances;
  const Texture& texture;
  Shader& shader = ResourceManager::getShader(SHADER_INDEX::TEXTURE);

  uint VAO, VBO, IBO;

  TextureBatch(const Texture& t) : texture(t) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float tx = 1.0f / texture.textureCount;

    float vertices[] = {
      // positions     // texture coords
      -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
      -0.5f,  0.5f,    0.0f, 1.0f, // top left 
       0.5f, -0.5f,    tx,   0.0f, // bottom right
       0.5f,  0.5f,    tx,   1.0f, // top right
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    {// global attributes
      // position attribute
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
      // texture coord attribute
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    glGenBuffers(1, &IBO);
  }

  ~TextureBatch() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
  }

  void view(View& view) {
    _view = &view;
    shader.use();
    shader.setMat4("projection", view.proj());
  }

  View& view() {
    return *_view;
  }

  void clear() {
    instances.clear();
  }

  void add(Instance&& instance) {
    instance.texOffset /= (float) texture.textureCount;
    instances.push_back(instance);
  }

  void update() {
    glBindBuffer(GL_ARRAY_BUFFER, IBO);
    glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(Instance), instances.data(), GL_STATIC_DRAW);

    int currentAttr = 2;
    size_t dataOffset = 0;
    auto addInstancedAttribute = [&](int size){
      glEnableVertexAttribArray(currentAttr);
      glVertexAttribPointer(currentAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)dataOffset);
      glVertexAttribDivisor(currentAttr, 1);
      currentAttr++;
      dataOffset += size * sizeof(float);
    };
    addInstancedAttribute(2);
    addInstancedAttribute(2);
    addInstancedAttribute(4);
    addInstancedAttribute(1);
    addInstancedAttribute(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void draw() {
    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture.id);

    // render container
    shader.use();
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instances.size());
  }
};