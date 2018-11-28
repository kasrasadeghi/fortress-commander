// From https://learnopengl.com/In-Practice/Text-Rendering

// GL includes
#include "ResourceManager.h"
#include "Shader.h"
#include "View.h"

// Std. Includes
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
// GLEW
// #define GLEW_STATIC
// #include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
  GLuint TextureID;   // ID handle of the glyph texture
  glm::ivec2 Size;    // Size of glyph
  glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
  GLuint Advance;     // Horizontal offset to advance to next glyph
};

class TextRenderer {
  std::map<GLchar, Character> _characters;
  GLuint _VAO;
  GLuint _VBO;
  Shader& _shader = ResourceManager::getShader(SHADER_INDEX::TEXT);

public:
  TextRenderer(View& view) {

    // Set OpenGL options
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Compile and setup the shader
    _shader.use();
    _shader.setMat4("projection", view.proj());

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
      std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
      std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++) {
      // Load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }
      // Generate texture
      GLuint texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0,
                   GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
      // Set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // Now store character for later use
      Character character = {texture,
                             glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                             glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                             static_cast<GLuint>(face->glyph->advance.x)};
      _characters[c] = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color) {
    // Activate corresponding render state
    _shader.use();
    _shader.setVec4("textColor", color);
    // glUniform3f(glGetUniformLocation(_shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
      Character ch = _characters[*c];

      GLfloat xpos = x + ch.Bearing.x * scale;
      GLfloat ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

      GLfloat w = ch.Size.x * scale;
      GLfloat h = ch.Size.y * scale;

      // clang-format off
      // Update VBO for each character
      GLfloat vertices[6][4] = {
        {xpos,     ypos - h, 0, 0},   
        {xpos,     ypos,     0, 1},
        {xpos + w, ypos - h, 1, 0},

        {xpos + w, ypos - h, 1, 0},
        {xpos,     ypos,     0, 1},
        {xpos + w, ypos,     1, 1},
      };
      // clang-format on

      // Render glyph texture over quad
      glBindTexture(GL_TEXTURE_2D, ch.TextureID);
      // Update content of VBO memory
      glBindBuffer(GL_ARRAY_BUFFER, _VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
      // Be sure to use glBufferSubData and not glBufferData

      glBindBuffer(GL_ARRAY_BUFFER, 0);
      // Render quad
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
      x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide
                                      // amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
};