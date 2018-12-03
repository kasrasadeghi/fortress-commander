#version 330 core
// non instanced
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 texCoord;

// instanced
layout (location = 2) in vec2 posOffset;
layout (location = 3) in vec2 size;
layout (location = 4) in vec4 aColor;
layout (location = 5) in float texOffset;
layout (location = 6) in float rotation;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 projection;

void main() {
	// output to fragmentShader
	ourColor = aColor;
	TexCoord = texCoord + vec2(texOffset, 0);

	mat4 scaleMatrix = mat4(1.0);
  mat4 rotateMatrix = mat4(1.0);
  mat4 translateMatrix = mat4(1.0);

  // rotate on Z axis
  rotateMatrix[0][0] = cos(rotation);
  rotateMatrix[1][0] = sin(rotation);
  rotateMatrix[0][1] = -sin(rotation);
  rotateMatrix[1][1] = cos(rotation);

  // scale
  scaleMatrix[0][0] = size[0];
  scaleMatrix[1][1] = size[1];

  // translate
  translateMatrix[3][0] = posOffset[0];
  translateMatrix[3][1] = posOffset[1];

	gl_Position = scaleMatrix * vec4(aPos, 0.0, 1.0);
  gl_Position = rotateMatrix * gl_Position;
  gl_Position = translateMatrix * gl_Position;
  gl_Position = projection * gl_Position; 
}