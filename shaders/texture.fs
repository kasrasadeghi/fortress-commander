#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = ourColor * texture(texture1, TexCoord);
}
