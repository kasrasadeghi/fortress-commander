#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 currpos;
layout (location = 2) in vec4 color;

varying vec4 instance_color;

uniform vec2 size;
uniform mat4 projection;

void main()
{
  mat4 model = mat4(1.0);

  // scale
  model[0][0] = size[0];
  model[1][1] = size[1];

  // translate
  model[3][0] = currpos[0];
  model[3][1] = currpos[1];

  instance_color = color;
  gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}