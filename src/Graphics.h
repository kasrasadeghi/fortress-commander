#pragma once

#include "Graphics/Shader.h"
#include "Graphics/RenderWindow.h"

#include <iostream>



class TricolorTriangleShape {
  std::array<float, 18> _vertices;
  const Shader _tricolor_shader{"shaders/tricolor_triangle.vs", "shaders/tricolor_triangle.fs"};

  void _create() {
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(),
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }

public:
  GLuint VAO;

  TricolorTriangleShape(std::array<float, 18> vertices) : _vertices(vertices) { _create(); }

  void draw() {
    _tricolor_shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
};

class UnicolorTriangleShape {
  std::array<float, 6> _vertices;
  glm::vec3 _color{0.f, 0.f, 0.f};
  Shader _unicolor_shader{"shaders/unicolor_triangle.vs", "shaders/unicolor_triangle.fs"};

  void _create() {
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(),
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  }

public:
  GLuint VAO;

  UnicolorTriangleShape(std::array<float, 6> vertices) : _vertices(vertices) { _create(); }

  void setColor(GLfloat r, GLfloat g, GLfloat b) {
    _color = glm::vec3(r, g, b);
  }

  void draw() {
    _unicolor_shader.use();
    _unicolor_shader.setVec3("color", _color);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
};