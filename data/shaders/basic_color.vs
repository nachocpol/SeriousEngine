#version 330 core

//Default attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

//Default uniforms
uniform mat4 uModel;

layout(std140)uniform uDrawData
{
	mat4 uView;
	mat4 uProjection;
};

out vec3 fragPos; 

void main()
{
	vec4 pos = uModel * vec4(position,1.0);
	
	fragPos = pos.xyz;
	
	//Calculate the final position of the vertex
	gl_Position = uProjection * uView * pos;
}