#version 330 core

uniform vec3 uColor;

in vec3 fragPos;

//Final fragment color
out vec4 color;

void main()
{	
	color = vec4(uColor, 1.0);
}