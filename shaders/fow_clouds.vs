#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoord;

out vec2 screen_pos;
out vec2 TexCoord;

uniform mat4 projection;

void main() {
  screen_pos = vertex;
  TexCoord = texCoord;
  gl_Position = projection * vec4(vertex, 0.0, 1.0);
}

