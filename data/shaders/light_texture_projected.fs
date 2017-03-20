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
uniform sampler2D uTexture;
uniform sampler2D uLightTexture;
uniform vec3 uColor;
uniform vec3 uSunPos;

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
    float bias = max(0.05 * (1.0 - dot(normal, uLights[id].lDir)), 0.005);      

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

float GetSpotAttenuation(Light l)
{
    float at = 0.01;
    vec3 lightDir = normalize(l.lPosition - fragPos);
    float theta = dot(lightDir, normalize(-l.lDir)); 
    
    if(theta > l.lCutOff) 
    {    
        // Attenuation
        float distance = length(l.lPosition - fragPos);
        at = 1.0f / (1.0 + 0.09 * distance + 0.032 * (distance * distance));    
    }
    return at;
}

void main()
{	
	vec3 colorTexture = texture(uTexture, texCoord).rgb;

	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(uSunPos - fragPos);
	float lambertian = max(dot(lightDir, normal), 0.0);
	colorTexture = colorTexture * lambertian;

	float attenuation = 1.0;
	if(uLights[0].lColor.w == 1.0)
	{
		attenuation = 3.0 * GetSpotAttenuation(uLights[0]);
	}
	colorTexture = colorTexture * attenuation;
	
	vec4 lPos = uLights[0].lMatrix * vec4(fragPos, 1.0);
	float visibility = 1.0 - GetShadow(lPos, 0);
	
	vec3 projCoords = lPos.xyz / lPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	vec4 lightTexture = texture(uLightTexture, projCoords.xy);
	
	visibility = clamp(visibility, 0.0, 1.0);
	
	vec3 finalColor = colorTexture.rgb;
	color = (mix(lightTexture, vec4(finalColor, 1.0), 1.2 - attenuation) * visibility);
	//color = vec4(finalColor, 1.0);
}