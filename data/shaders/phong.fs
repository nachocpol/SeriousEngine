#version 330 core

in vec2 tc;
in vec3 norm;
in vec3 pos;

uniform sampler2D diffuse;

//Final fragment color
out vec4 color;

vec3 CalcColor()
{
	vec3 LightPosition = vec3 (100.0,0.0,100.0);
	vec3 LightIntensity = vec3(0.7,0.7,0.7);
	
	//Ambient color
	vec3 Ka = vec3(0.8, 0.8, 0.8);
	//Diffuse color
	vec3 Kd = vec3(0.8,0.8,0.8);
	//Specular color
	vec3 Ks = vec3(0.9, 0.9, 0.9);
	float Shininess = 15.0;

	//We dont need the magnitude of the normal, just the direction, so we normalize it
	vec3 n = normalize(norm);
	//Vector from the light pos to this fragment position (again we just need the direction)
	vec3 s = normalize( vec3(LightPosition) - pos );
	vec3 v = normalize(vec3(-pos));//BAD!!! we need the camera position
	//We calculate how much we reflec using the normal and the -light direction
	vec3 r = reflect( -s, n );

	//Ambient + diffuse + specular	
	return LightIntensity *(	Ka +
 								Kd * max( dot(n, s), 0.0 ) +
	 							Ks * pow( max( dot(v,r), 0.0 ), Shininess ) );
}

void main()
{
	color = vec4(CalcColor(),1.0);
	//0.8:the intensity of the texture
	color *= texture(diffuse,tc)*0.8; 
	//color = vec4(1.0,0,0,1.0);
	//color = vec4(norm,1.0);
}