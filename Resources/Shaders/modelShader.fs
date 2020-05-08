precision mediump float;

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_toLightVec;
varying vec3 v_toCameraVec;
varying vec3 v_FragPos;

uniform sampler2D u_texture;
uniform vec3 u_lightColorDif;
uniform vec3 u_lightColorSpec;
uniform float u_lightType;
uniform float u_shineDamper;
uniform float u_reflectivity;
uniform vec3 u_cameraPos;


struct Material {
vec3 ambient;
vec3 diffuse;
vec3 specular;
float shininess;
};

struct SpotLight {
vec3 direction;
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 color;

float constant;
float linear;
float quadratic; 
float cutOff;
float outterCutOff;

};

struct DirLight {
vec3 direction;
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 color;
};

struct PointLight {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 color;

float constant;
float linear;
float quadratic; 
};

//uniform DirLight light;
uniform Material material;
uniform SpotLight pLight;

vec3 CalcSpotLight (SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{	
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));

	if(theta > light.cutOff)
	{
		float eps = (light.cutOff - light.outterCutOff);
		float intensity = clamp ((theta-light.outterCutOff)/eps, 0.0,1.0);
		// diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
		// specular shading
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
		// attenuation
		float distance    =  length(light.position - fragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + 
  				     light.quadratic * (distance * distance));    
		// combine results
		vec3 ambient  = light.ambient  * material.ambient;
		vec3 diffuse  = light.diffuse  * diff * material.diffuse;
		vec3 specular = light.specular * spec * material.specular;

		ambient  *= intensity;
		diffuse  *= intensity;
		specular *= intensity;

		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;
		return (ambient + diffuse + specular);
	} 
	else
	{ 
		return light.ambient * material.ambient;
	}
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
    // attenuation
    float distance    =  length(light.position - fragPos);
     float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * material.ambient;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	//diffuse calculation
	float diff = max(dot(normal,lightDir),0.0);
	//specular
	vec3 reflectDir = reflect(-lightDir,normal);
	float specFactor = pow(max(dot(viewDir, reflectDir),0.0),material.shininess*128.0);

	vec3 ambient = light.ambient * material.ambient * light.color;
	vec3 diffuse = light.diffuse * diff * material.diffuse * light.color;
	vec3 spec = light.specular * specFactor * material.specular * light.color;

	return (ambient+diffuse+spec); 
}

void main()
{	
	vec3 normalizedNormal = normalize(v_normal);
	vec3 normalizedToLightVec = normalize(v_toLightVec);	
	vec3 normalizedViewDir = normalize (u_cameraPos - v_FragPos);
	
	 vec4 texColor = texture2D(u_texture,v_uv);
	  
	// vec4 finalColor = vec4(CalcDirLight(light,normalizedNormal,normalizedViewDir) * texColor.xyz,1.0);	
	 vec4 finalColor = vec4(CalcSpotLight(pLight,normalizedNormal,v_FragPos,normalizedViewDir) * texColor.xyz,1.0);	

	gl_FragColor = finalColor;	
}
