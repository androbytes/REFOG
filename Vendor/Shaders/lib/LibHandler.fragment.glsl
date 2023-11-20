#version 460 core

uniform uint Extensions[16];

extern vec4 ProcessLight(vec3 color, vec2 texCoord);

vec4 HandleLib(vec2 texCoord)
{
	vec4 result = vec4(1.0f);

	for(int i = 0; i < 16; i++)
	{
		switch(Extensions[i])
		{
			case 0:
				continue;
				break;
			case 1:
				result *= ProcessLight(result.xyz, texCoord);
				break;
		}
	}

	return result;
}