#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 texCoord;

uniform mat4 Model;
uniform mat4 ViewProjection;

void main()
{
	gl_Position = ViewProjection * Model * vec4(aPos, 1.0);
	vertexColor = vec4(aCol, 1.0);
	//texCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
	texCoord = aTexCoord;
}