#version 330 core

#define MAX_LIGHTS 5

struct Light
{
	vec4 lColor;
	vec3 lDir;
	mat4 lMatrix;
	vec3 lPosition;
    float lCutOff;
};

uniform sampler2D uShadowMaps[MAX_LIGHTS];
uniform Light[MAX_LIGHTS] uLights;
uniform int uCurLights;
float Shininess = 100.0;

uniform vec3 uColor;
uniform vec3 uSunPos;
uniform vec3 uCamPos;

in vec2 texCoord;
in vec3 fragPos;
in vec3 norm;

//Final fragment color
out vec4 color;

float GetShadow(vec4 lPos, int id)
{
    vec3 normal = normalize(norm);

    vec3 projCoords = lPos.xyz / lPos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(uShadowMaps[id], projCoords.xy).r; 
    float currentDepth = projCoords.z;

    //Calculate bias
    float bias = max(0.05 * (1.0 - dot(normal, -uLights[id].lDir)), 0.005);      

    //PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMaps[id], 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uShadowMaps[id], projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }

    shadow /= 9.0;
    return shadow;
    //return abs(shadow-1.0);
}

void main()
{	
	vec3 lightDir = normalize(uSunPos - fragPos);
	
	float d = distance(fragPos, uSunPos) * 0.08;
	
	d = 1.0 - clamp(d, 0.0, 1.0);

	vec3 normal = normalize(norm);
	float lambertian = clamp(dot(normal, lightDir), 0.0, 1.0);
	float specular = 0.0;
	if (lambertian > 0.0)
	{
		vec3 viewDir = normalize(uCamPos - fragPos);
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, normal), 0.0);
		specular = pow(specAngle, Shininess);
	}
	
	float visibility = 1.0;
	visibility -= (1.0/uCurLights) * GetShadow(uLights[0].lMatrix * vec4(fragPos, 1.0), 0);
	
	vec3 finalColor = uColor * lambertian;
	finalColor += specular;
	finalColor *= d;
	finalColor *= visibility;
	
	color = vec4(finalColor, 1.0);
	//color = vec4(vec3(visibility), 1.0);
}