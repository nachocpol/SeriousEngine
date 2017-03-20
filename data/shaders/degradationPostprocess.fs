#version 430 core

in vec2 oUv;

uniform sampler2D uScreenTexture;

out vec4 color;

void main()
{
	vec4 texColor = texture(uScreenTexture, oUv);
	
	vec3 greyScene = vec3((texColor.r + texColor.g + texColor.b) / 3.0);
	vec3 greenScene = vec3(0.0f, texColor.r, 0.0f);
	
	vec3 finalColor = mix(greenScene, greyScene, oUv.x);
	
	color = vec4(vec3(finalColor), 1.0);
}
