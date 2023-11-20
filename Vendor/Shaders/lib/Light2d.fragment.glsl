#version 460 core

uniform float cutoff;
uniform vec2 lightPos;
uniform sampler2D lightTex;

// Multiply the light pos like this: lightPos = (model * vec4(aPos, 1.0)).xy
vec4 ProcessLight(vec3 color, vec2 texCoord)
{
	vec4 returnColor = vec4(1.0f);
	float dist = distance(gl_FragCoord.xy, lightPos);
	dist = clamp(dist, cutoff, 1.0f);
	if(dist == cutoff){
		dist = 0;
	}
	dist = 1 - dist;
	returnColor = vec4(color.xyz, dist) * texture(lightTex, texCoord);
	return returnColor;
}