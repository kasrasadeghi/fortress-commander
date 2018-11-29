#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 currpos;
layout (location = 2) in vec4 color;
layout (location = 3) in float rotation;
layout (location = 4) in vec2 size;

varying vec4 instance_color;

uniform mat4 projection;

void main()
{
  mat4 scaleMatrix = mat4(1.0);
  mat4 rotateMatrix = mat4(1.0);
  mat4 translateMatrix = mat4(1.0);

  // rotate on Z axis
  rotateMatrix[0][0] = cos(rotation);
  rotateMatrix[1][0] = sin(rotation);
  rotateMatrix[0][1] = -sin(rotation);
  rotateMatrix[1][1] = cos(rotation);

  // scale
  scaleMatrix[0][0] = size[0];
  scaleMatrix[1][1] = size[1];

  // translate
  translateMatrix[3][0] = currpos[0];
  translateMatrix[3][1] = currpos[1];

  instance_color = color;
  gl_Position = scaleMatrix * vec4(vertex.xy, 0.0, 1.0);
  gl_Position = rotateMatrix * gl_Position;
  gl_Position = translateMatrix * gl_Position;
  gl_Position = projection * gl_Position; 
}