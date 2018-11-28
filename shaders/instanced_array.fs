#version 330 core
out vec4 FragColor;  

varying vec4 instance_color;

void main() {
  FragColor = instance_color;
}