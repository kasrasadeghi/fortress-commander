#version 330 core
out vec4 FragColor;

uniform int num_lights;
uniform vec2[1024] positions;
uniform vec4[1024] colors;
uniform float[1024] intensities;

varying vec2 screen_pos;

void main() {
  vec3 color = vec3(0.2);
  for (int i = 0; i < num_lights; i++) {
    float dist = distance(screen_pos, positions[i]) / intensities[i] + 1.;
    float f = clamp(1.f / (dist * dist), 0, 1);
    color += colors[i].rgb * f * colors[i].a;
  }
  FragColor = vec4(clamp(color, vec3(0), vec3(1)), 1.);
}