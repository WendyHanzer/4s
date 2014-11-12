#version 300 es

precision mediump float;

const int MAX_POINT_LIGHTS = 8;
const int MAX_SPOT_LIGHTS = 8;
const int MAX_DIRECTIONAL_LIGHTS = 8;

struct BaseLight
{
	vec3 color;					//color of light
	float diffuseIntensity;		//intensity of light
	float ambientIntensity;
}; //BaseLight

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;				//direction of light
}; //DirectionalLight

//light/(constant + linear*distance + exp*distance^2)
struct Attenuation
{
	float constant;
	float linear;
	float exp;
}; //Attenuation

struct PointLight
{
	BaseLight base;
	vec3 position;				//position of light
	Attenuation atten;			//weakening of light
}; //PointLight

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float cutoff;				//cos(angle in radians defining cone)
}; //SpotLight

uniform int numDirectionalLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform DirectionalLight directionalLight[MAX_POINT_LIGHTS];
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

uniform vec4 materialDiffuse;		//diffuse intensity
uniform vec4 materialAmbient;		//ambient intensity
uniform vec4 materialSpecular;		//specular intensity
uniform float materialShine;		//specular power

uniform vec3 cameraPosition;		//camera position

uniform sampler2D sampler;			//texture

in vec3 fNormal;
in vec3 fPosition;
in vec2 fUV;

out vec4 color;

vec4 calcLightInternal(in BaseLight light, in vec3 lightDirection, in vec3 normal)
{
	vec4 ambientColor = vec4(light.color, 1.0f)*light.ambientIntensity;

	float diffuseFactor = dot(normal, lightDirection);

	vec4 diffuseColor  = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0f)
	{
		diffuseColor = vec4(light.color, 1.0f)*light.diffuseIntensity*diffuseFactor;

		vec3 lightReflect = normalize(reflect(-lightDirection, normal));
		vec3 viewDirection = normalize(cameraPosition - fPosition);
		float specularFactor = dot(lightReflect, viewDirection);
		specularFactor = pow(specularFactor, materialShine);
        
		if (specularFactor > 0.0f)
		{
			specularColor = vec4(light.color, 1.0f)*specularFactor;
		} //if
	} //if
	
    return (ambientColor*materialAmbient + diffuseColor*materialDiffuse + specularColor*materialSpecular);
} //calcLightInternal

vec4 calcDirectionalLight(in DirectionalLight directionalLight, in vec3 normal)
{
	return calcLightInternal(directionalLight.base, directionalLight.direction, normal);  
} //calcDirectionalLight


vec4 calcPointLight(in PointLight light, in vec3 normal)
{
	vec3 lightDirection = light.position - fPosition;
	float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);
    
	vec4 color = calcLightInternal(light.base, lightDirection, normal);
	float attenuation =  light.atten.constant +
							light.atten.linear*distance +
							light.atten.exp*distance*distance;
	
	return color/attenuation;
} //calcPointLight

vec4 calcSpotLight(in SpotLight light, in vec3 normal)
{
    vec3 lightToPixel = normalize(fPosition - light.base.position);
    float spotFactor = dot(lightToPixel, light.direction);
    
	if (spotFactor > light.cutoff)
	{
		return calcPointLight(light.base, normal);
	
		vec4 color = calcPointLight(light.base, normal);
		return color*(1.0f - (1.0f - spotFactor)*1.0f/(1.0f - light.cutoff));
	} //if
	else
	{
		return vec4(0f, 0f, 0f, 0f);
	} //else
} //calcSpotLight

void main()
{
	vec3 normal = normalize(fNormal);
	
	vec4 totalLight = vec4(0.0f);

	for (int i = 0; i < numDirectionalLights; ++i)
	{                                           
		totalLight += calcDirectionalLight(directionalLight[i], fNormal);
	} //for
	
	for (int i = 0; i < numPointLights; ++i)
	{
		totalLight += calcPointLight(pointLight[i], normal);
	} //for
	
	for (int i = 0; i < numSpotLights; ++i)
	{
		totalLight += calcSpotLight(spotLight[i], normal);
	} //for
	
	color = texture(sampler, fUV)*totalLight;
} //main
