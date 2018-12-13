#version 330 core
in vec2 screen_pos;
in vec2 instance_pos;
in vec4 instance_color;
in float instance_intensity;

out vec4 FragColor;

const float distance_factor = 3.f;
const float border_radius = 4.0f;

void main() {
  float dist = distance(screen_pos, instance_pos) / instance_intensity;
  float f = clamp((distance_factor + border_radius - dist) / border_radius, 0, 1);
  FragColor = vec4(vec3(f), 0);
}
