#pragma once

#include <glad/glad.h>

#include "GL.h"

struct TexturedRectangle {
  Shader& shader;
  GL::VertexArray VA;

  TexturedRectangle(glm::vec2 a, glm::vec2 b, 
                    Shader& shader = ResourceManager::getShader(SHADER_INDEX::SIMPLE_TEXTURE)) 
                    : shader(shader) {

    std::vector<glm::vec4> vertices = {
        // positions     // texture coords
        {a.x, b.y, 0.0f, 1.0f}, // bottom left
        {a.x, a.y, 0.0f, 0.0f}, // top left
        {b.x, b.y, 1.0f, 1.0f}, // bottom right
        {b.x, a.y, 1.0f, 0.0f}, // top right
    };

    VA.VB.bindData(vertices);

    VA.setAttributeLayout({2, 2});
  }

  /**
   * @brief Draw the textured rectangle using the texture already bound to GL_TEXTURE_2D
   */
  void draw(View& view) {
    // render 
    shader.use();
    shader.setMat4("projection", view.proj());
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VA.VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
  }
};