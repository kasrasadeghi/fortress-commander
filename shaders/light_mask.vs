#version 330 core
layout (location = 0) in vec2 vertex;

uniform mat4 projection;

varying vec2 screen_pos;

void main() {
  screen_pos = vertex;
  gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
}