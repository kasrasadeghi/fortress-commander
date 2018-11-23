#include "Instanced.h"

GLuint InstancedRectangle::_VAO;

const GLfloat InstancedRectangle::_base_vertices[12] = {
  // positions   
  // -0.05f,  0.05f,
  // 0.05f, -0.05f, 
  // -0.05f, -0.05f,
  
  // -0.05f,  0.05f,
  // 0.05f, -0.05f, 
  // 0.05f,  0.05f,


  0.f, 1.f,
  1.f, 0.f, 
  0.f, 0.f,
  
  0.f, 1.f,
  1.f, 0.f,
  1.f, 1.f,  
};