#pragma once

#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

struct Texture {
  uint id;
  int width;
  int height;
  float textureCount;

  Texture(std::string filename = "textures/textures.png", bool wrap = false) {
    // load and create a texture
    glGenTextures(1, &id);
    glBindTexture(
        GL_TEXTURE_2D,
        id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP_TO_BORDER);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int nrChannels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 4);

    if (nrChannels == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (nrChannels == 4) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    if (data) {
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    textureCount = (float)width / (float)height;
  }

  void bind() {
    glBindTexture(GL_TEXTURE_2D, id);
  }

  void unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
};
