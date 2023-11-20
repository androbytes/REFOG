#version 460 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D Texture;

void main()
{
	FragColor = texture(Texture, texCoord) * vertexColor;
}