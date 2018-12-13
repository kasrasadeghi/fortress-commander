#pragma once

#include <glad/glad.h>

namespace GL {

struct VertexBuffer {
  uint VBO;

  explicit VertexBuffer() {
    glGenBuffers(1, &VBO);
  }

  ~VertexBuffer() {
    glDeleteBuffers(1, &VBO);
  }

  void bind() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
  }

  static void unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  template <typename T>
  void bindData(const std::vector<T>& vertices) {
    // assert: sum(attribute sizes) * sizeof(float) == sizeof(T)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
};

struct VertexArray {
private:
public:
  uint attribCounter = 0;

  VertexBuffer VB; // vertex buffer
  VertexBuffer IB; // instance buffer
  uint VAO;

  VertexArray() {
    glGenVertexArrays(1, &VAO);
  }

  ~VertexArray() {
    glDeleteVertexArrays(1, &VAO);
  }

  void bind() {
    glBindVertexArray(VAO);
  }

  static void unbind() {
    glBindVertexArray(0);
  }

  VertexBuffer& buffer(bool instanced = false) {
    return instanced ? IB : VB;
  }

  /**
   * Takes a list of attribute sizes and creates a buffer with that layout.
   *
   * @returns the vertex buffer that was created so that you can load data into it.
   */
  void setAttributeLayout(std::initializer_list<uint> sizes, bool instanced = false) {
    bind();

    buffer(instanced).bind();

    size_t stride =
        0; // the stride should be the size of the elements of the vertex buffer == sum of sizes
    for (uint size : sizes) {
      stride += size;
    }

    size_t dataOffset = 0;

    for (uint size : sizes) {
      glEnableVertexAttribArray(attribCounter);
      glVertexAttribPointer(attribCounter, size, GL_FLOAT, GL_FALSE, stride * sizeof(float),
                            (void*)dataOffset);
      if (instanced) {
        glVertexAttribDivisor(attribCounter, 1);
      }
      attribCounter++;
      dataOffset += size * sizeof(float);
    }

    VertexBuffer::unbind();
    unbind();
  }
};

struct RenderTexture {
private:
  glm::ivec2 _texDim;

public:
  uint fboId;
  uint texId;

  RenderTexture() {
    glGenFramebuffers(1, &fboId);
    glGenTextures(1, &texId);
  }

  ~RenderTexture() {
    glDeleteTextures(1, &texId);
    glDeleteFramebuffers(1, &fboId);
  }

  /**
   * @brief Bind framebuffer for rendering to texture
   */
  void bindFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
  }

  /**
   * @brief Unbind framebuffer for resetting render target
   */
  void unbindFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  /**
   * @brief Bind texture for usage of rendered data in shaders, etc.
   */
  void bindTexture() {
    glBindTexture(GL_TEXTURE_2D, texId);
  }

  /**
   * @brief Unbind texture
   */
  void unbindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void updateTextureDimensions(int w, int h) {
    if (w != _texDim.x || h != _texDim.y) {
      _texDim = {w, h};
      bindFramebuffer();
      bindTexture();
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      unbindTexture();

      // attach texture to framebuffer
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);

      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	      std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

      unbindFramebuffer();
    }
  }
};
} // namespace GL
