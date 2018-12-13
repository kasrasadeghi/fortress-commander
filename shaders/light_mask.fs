#version 330 core
in vec2 screen_pos;
in vec2 instance_pos;
in vec4 instance_color;
in float instance_intensity;

out vec4 FragColor;

void main() {
  float dist = distance(screen_pos, instance_pos) / instance_intensity + 1.;
  float f = clamp(1.f / (dist * dist), 0, 1);
  vec3 color = instance_color.rgb * f * instance_color.a;
  FragColor = vec4(clamp(color, vec3(0), vec3(1)), 1.);
}
