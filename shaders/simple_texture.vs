#version 330 core
// non instanced
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 projection;

void main() {
	// output to fragmentShader
	TexCoord = texCoord;

  gl_Position = projection * vec4(aPos, 0.0, 1.0); 
}