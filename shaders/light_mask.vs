#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 pos;
layout (location = 2) in vec4 color;
layout (location = 3) in float intensity;

out vec2 screen_pos;
out vec2 instance_pos;
out vec4 instance_color;
out float instance_intensity;

uniform mat4 projection;

void main() {
  screen_pos = vertex;
  instance_pos = pos;
  instance_color = color;
  instance_intensity = intensity;
  gl_Position = projection * vec4(vertex, 0.0, 1.0);
}
