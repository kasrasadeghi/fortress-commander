#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 currpos;
layout (location = 2) in vec4 color;
layout (location = 3) in float rotation;
layout (location = 4) in vec2 size;

varying vec4 instance_color;

uniform mat4 projection;

void main()
{
  mat4 rotationMatrix = mat4(1.0);
  mat4 model = mat4(1.0);

  // rotate on Z axis
  rotationMatrix[0][0] = cos(rotation);
  rotationMatrix[1][0] = sin(rotation);
  rotationMatrix[0][1] = -sin(rotation);
  rotationMatrix[1][1] = cos(rotation);

  // scale
  model[0][0] = size[0];
  model[1][1] = size[1];

  // translate
  model[3][0] = currpos[0];
  model[3][1] = currpos[1];

  instance_color = color;
  gl_Position = projection * model * (rotationMatrix * vec4(vertex.xy, 0.0, 1.0));
}