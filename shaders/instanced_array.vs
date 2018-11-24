#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 currpos;

uniform vec2 size;
uniform mat4 projection;

void main()
{
  // mat4 model = mat4(
  //   size.x,    0.0,       0.0,      0.0,
  //   0.0,       size.y,    0.0,      0.0, 
  //   0.0,       0.0,       1.0,      0.0,
  //   currpos.x, currpos.y, 0.0,      1.0
  // );

  mat4 model = mat4(1.0);

  model[0][0] = size[0];
  model[1][1] = size[1];

  model[3][0] = currpos[0];
  model[3][1] = currpos[1];

  gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}