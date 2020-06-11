precision mediump float;

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_toCameraVec;
varying vec3 v_FragPos;
varying float v_visibility;

uniform sampler2D u_texture;
uniform vec3 u_cameraPos;
uniform vec3 u_skyColor;

#define MAX_LIGHTS 4

struct Material {
vec3 ambient;
vec3 diffuse;
vec3 specular;
float shininess;
};

struct Light {
vec3 direction;
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 color;

float range;
float intensity;
float cutOff;
float outterCutOff;

int type;
};

//uniform DirLight light;
uniform Material material;
uniform Light lights[MAX_LIGHTS];

vec3 CalcSpotLight (Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{	
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float distance    =  length(light.position - fragPos);

	if(light.range>distance)
	{ 
		if (theta > light.cutOff)
		{
		
		float eps = (light.cutOff - light.outterCutOff);
		float intensity = clamp ((theta-cos(light.outterCutOff))/eps, 0.0,1.0);
		float tempIntensity = max(pow((theta - cos(light.outterCutOff/2.0))/(cos(light.cutOff/2.0)- cos(light.outterCutOff/2.0)),5.0), 0.0);
		// diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
		// specular shading
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
		// attenuation
		//float attenuationCircle = 1.0 - light.cutOff/light.outterCutOff  - (light.cutOff*light.cutOff)/(light.outterCutOff* light.outterCutOff);
		float attenuationRange = 1.0 - (distance * distance)/(light.range*light.range); 
		
		vec3 ambient  = light.ambient;
		vec3 diffuse  = light.diffuse  * diff;
		vec3 specular = light.specular * spec;		
	
		ambient  *= light.intensity*tempIntensity*attenuationRange;
		diffuse  *= light.intensity*tempIntensity*attenuationRange;
		specular *= light.intensity*tempIntensity*attenuationRange;


		return (ambient + diffuse + specular);
		}
		else
		{
		return vec3(0);
		}
	} 
	else
	{ 
		return vec3(0);
	}
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	float distance = length(light.position - fragPos);
	if (light.range > distance)
	{
	vec3 lightDir = normalize(light.position - fragPos);	
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
    // attenuation    
	float attenuation = 1.0 - distance/light.range  - (distance * distance)/(light.range*light.range);   

	//float attenuation = 1.0 / ((1.0 + 0.1 * distance + 0.01* (distance * distance)));   
	//EXPERIMENTAL: attenuation += max(1.0-(distance*distance)/(light.range*light.range),0.0);	
	//attenuation += max(1.0-(distance*distance)/(light.range*light.range),0.0);	
    // combine results
    vec3 ambient  = light.ambient ;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;	

	float intensity = light.intensity;

	ambient  *= intensity;
	diffuse  *= intensity;
	specular *= intensity;	

    return (ambient + diffuse + specular);
	}
	else
	{
	return vec3(0);
	}
}

vec3 CalcDirLight(Light light, vec3 norm, vec3 viewDir)
{
	vec3 ambient = light.ambient;
  	
    // diffuse     
    // vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff;  
    
    // specular   
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
    vec3 specular = light.specular * spec;  	
        
    return ambient + diffuse + specular;	
}

void main()
{	
	//Point = 0, Directional 1, Spot 2
	vec3 normalizedNormal = normalize(v_normal);
	vec3 normalizedViewDir = normalize(u_cameraPos - v_FragPos);
	
	vec3 texColor = texture2D(u_texture,v_uv).xyz;
	vec3 finalResult = vec3(0);	 	

	for (int i=0; i<MAX_LIGHTS; i++)
	{
		if (lights[i].type == 0)
		{
			finalResult+=CalcPointLight(lights[i],normalizedNormal,v_FragPos, normalizedViewDir);
		}
		if (lights[i].type == 1)
		{
			finalResult+=CalcDirLight(lights[i],normalizedNormal, normalizedViewDir);
		}
		if (lights[i].type == 2)
		{
			finalResult+=CalcSpotLight(lights[i],normalizedNormal,v_FragPos, normalizedViewDir);
		}	 	
	}	
	gl_FragColor = vec4(finalResult*texColor ,1.0);	
	gl_FragColor = mix(vec4(u_skyColor,1.0),gl_FragColor, v_visibility);
}