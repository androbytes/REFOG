#version 460 core
out vec4 FragColor;
in vec2 TexCoords;

uniform vec3 TextColor;
uniform sampler2D Texture;

void main()
{
	FragColor = vec4(TextColor, texture(Texture, TexCoords).r);
}