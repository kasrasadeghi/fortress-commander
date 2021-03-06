#version 330 core
out vec4 FragColor;

in vec2 screen_pos;
in vec2 TexCoord;

uniform sampler2D bufferTexture;
uniform sampler2D cloudTexture;
uniform float time;

const vec3 baseColor = vec3(0.5);
const int layers = 3;
const float speed = 0.05;
const float scale = 0.05;
const float scale_per_layer = 0.67;

vec3 getCloudColor() {
  vec3 cloudColor = baseColor;
  float layerScale = scale;
  for (int i = 0; i < layers; i++) {
    float t = time * speed;
    vec2 cloudOffset = vec2(sin(i), cos(i)); // a base "random" offset to scatter layers
    cloudOffset += vec2(sin(i * 2.53 + t * 1.24) * 0.5 + t * 1.0, cos(i * 0.75 + t * 0.92) * 0.5); // time-based movement
    vec4 color = texture(cloudTexture, screen_pos * layerScale + cloudOffset);
    float f = color.a * (2.0 / layers);
    cloudColor = color.rgb * f + cloudColor * (1.0 - f); // alpha blending
    layerScale *= scale_per_layer;
  }
  return cloudColor;
}

void main() {
  vec3 cloudColor = getCloudColor();
  vec4 bufferColor = texture(bufferTexture, TexCoord);
  FragColor = vec4(cloudColor, 1.0 - bufferColor.r * (2.0 - cloudColor.r));

}
