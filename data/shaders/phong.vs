#version 330 core

//Default attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2)in vec3 normal;

//Default uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 tc;
out vec3 norm;
out vec3 pos;

void main()
{
	//The texture coordinate
	//tc = vec2(texcoord.x, 1.0 - texcoord.y);
	//We load in in the correct way
	tc = texcoord;

	//Model view matrix
	mat4 mv = model * view;
	//We take the normal matrix(uniform transform the normal)
	mat3 modelview = mat3
	(
		mv[0][0],mv[0][1],mv[0][2],
		mv[1][0],mv[1][1],mv[1][2],
		mv[2][0],mv[2][1],mv[2][2]
	);
	//Put the normal in view space
	norm = vec3(modelview * normal);

	//The position of the vert in world space
	pos = vec3(model * vec4(position,1.0));
	
	//Calculate the final position of the vertex
	gl_Position =  projection * view * model * vec4(position,1.0);
}