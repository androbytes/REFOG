#version 460 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D Texture;

extern vec4 HandleLib(vec2 texCoord);

void main()
{
	vec4 Result = texture(Texture, texCoord);
	//Result *= vertexColor;
	//Result *= HandleLib(texCoord);
	FragColor = Result;
}