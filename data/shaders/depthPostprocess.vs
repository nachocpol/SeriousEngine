#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 oUv;
out vec2 oDepthUv;

void main()
{
	gl_Position = vec4(position,1.0);
	oUv = vec2(uv.x,uv.y * -1);
	oDepthUv = vec2(uv.x, 1.0 - uv.y);
}