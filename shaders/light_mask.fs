#version 330 core
out vec4 FragColor;

uniform vec2[1024] positions;
uniform vec4[1024] colors;
uniform float[1024] intensities;

void main() {
  FragColor = vec4(1., 0., 1., 1.);
}