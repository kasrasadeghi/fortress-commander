#version 330 core
layout (location = 0) in vec2 vertex;

uniform vec2 size;
uniform mat4 projection;
uniform vec2 positions[100];

void main() {
  vec2 currpos = positions[gl_InstanceID];
  // mat4 current_model = model * 1;
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
  // mcopy = model;
  // mcopy;
  // model[3].xy = current_position.xy;
  // vec2 pos = projection * model * vec4(vertex.xy, 0.0, 1.0);
  // vec2 pos = current_position + vec4(vertex.xy, 0.0, 1.0);
  gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}