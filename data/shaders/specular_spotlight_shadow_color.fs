#version 330 core

#define MAX_LIGHTS 5

#define L_CONSTANT 1.0
#define L_LINEAR 0.09
#define L_QUADRATIC 0.032

#define PI 3.14159265359

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
uniform vec3 uCamPos;
uniform sampler2D uTexture;

in vec2 texCoord;
in vec3 fragPos;
in vec3 norm;

//Final fragment color
out vec4 color;

float GetSpotAttenuation(Light l)
{
    float at = 0.01;
    vec3 lightDir = normalize(l.lPosition - fragPos);
    float theta = dot(lightDir, normalize(-l.lDir)); 
    
    if(theta > l.lCutOff) 
    {    
        // Attenuation
        float distance = length(l.lPosition - fragPos);
        at = 1.0f / (L_CONSTANT + L_LINEAR * distance + L_QUADRATIC * (distance * distance));    
    }
    return at;
}

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

void main()
{	
	vec3 colorTexture = texture(uTexture, texCoord).rgb;

	vec3 normal = normalize(norm);
	
	vec3 eyeDir = normalize(uCamPos - fragPos);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
	vec3 lightcolor = vec3(0.0);
    float attenuation = 0.0;
	float acumAtt = 0.0;
	
	for(int i = 0; i < uCurLights; i++)
    {
        if(uLights[i].lColor.w == 0.0)
        {
            attenuation = 1.0;
			acumAtt = 1.0;
        }
		if(uLights[i].lColor.w == 1.0)
		{
			attenuation = (1.0/uCurLights) * GetSpotAttenuation(uLights[i]);
			acumAtt += (1.0/uCurLights) * GetSpotAttenuation(uLights[i]);
		}
		lightcolor += uLights[i].lColor.xyz * attenuation;
		
        float NdotL = clamp(dot(normal, -uLights[i].lDir), 0.0, 1.0);
		//diffuse  += NdotL * uLights[i].lColor.xyz;
		diffuse  += NdotL;
		
		float specular2 = 0.0;
		if (NdotL > 0.0)
		{
			vec3 viewDir = normalize(uCamPos - fragPos);
			vec3 halfDir = normalize(-uLights[i].lDir + viewDir);
			float specAngle = max(dot(halfDir, normal), 0.0);
			specular2 = pow(specAngle, Shininess);
		}
		
        specular += specular2;
	}
	
    float visibility = 1.0;
    for(int i=0; i < uCurLights; i++)
    {
		visibility -= (1.0/uCurLights) * GetShadow(uLights[i].lMatrix * vec4(fragPos, 1.0), i);
    }
	
	visibility = clamp(visibility, 0.0, 1.0);
	//if(visibility < 0.5) visibility = 0.0;
	
	vec3 finalColor = colorTexture * diffuse;
	finalColor += specular;
    finalColor *= acumAtt;
	finalColor += (lightcolor * 5.0) * visibility;
	
	color = vec4(finalColor, 1.0);
	//color = vec4(lightcolor * 10.0, 1.0);
	//color = vec4(visibility);
	
	//float d = distance(uCamPos, fragPos) * 0.05;
	//d = clamp(d, 0.0, 1.0);
	
	//vec3 greyColor = vec3((finalColor.r + finalColor.g + finalColor.b)/3.0);
	
	//color = vec4(vec3(d), 1.0);
	//color = vec4(greyColor * d, 1.0);
}