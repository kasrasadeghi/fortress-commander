#pragma once 

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

class View {
  glm::vec2 _center;
  glm::vec2 _radius;
  glm::mat4 _proj;

public:
  View& center(GLfloat cx, GLfloat cy) {
    _center = glm::vec2(cx, cy);
    return *this;
  }

  View& radius(GLfloat rx, GLfloat ry) {
    _radius = glm::vec2(rx, ry);
    return *this;
  }

  glm::mat4 proj() {
    _proj = glm::ortho(_center[0] - _radius[0], _center[0] + _radius[0], 
                       _center[1] - _radius[1], _center[1] + _radius[1], 
                       -1.f, 1.f);
    return _proj;
  }
};