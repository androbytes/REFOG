#version 460 core
layout (location = 0) in vec4 aPos; // 2 pos, 2 texCoords
out vec2 TexCoords;

uniform mat4 Projection;

void main()
{
	gl_Position = Projection * vec4(aPos.xy, 0.0, 1.0);
	TexCoords = aPos.zw;
}