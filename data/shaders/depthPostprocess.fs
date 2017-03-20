#version 430 core

in vec2 oUv;
in vec2 oDepthUv;

uniform sampler2D uScreenTexture;
uniform sampler2D uDepthTexture;

out vec4 color;

void main()
{
	float depthColor = texture(uDepthTexture, oDepthUv).r;
	vec4 texColor = texture(uScreenTexture, oUv);
	
	vec3 greyColor = vec3((texColor.r + texColor.g + texColor.b)/3.0);
	
	if(depthColor < 1.0)
		depthColor = dot(depthColor, 0.9);
	
	vec3 finalColor;
	
	finalColor.r = (texColor.r + (greyColor.r - texColor.r)) / depthColor;
	finalColor.g = (texColor.g + (greyColor.g - texColor.g)) / depthColor;
	finalColor.b = (texColor.b + (greyColor.b - texColor.b)) / depthColor;
	
	if(oUv.x > 0.5)
		color = color = vec4(finalColor, 1.0);
	else
		color = vec4(vec3(depthColor), 1.0);
}
