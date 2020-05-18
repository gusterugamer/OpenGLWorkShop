precision mediump float;

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_toCameraVec;
varying vec3 v_FragPos;

uniform sampler2D u_texture;
uniform vec3 u_cameraPos;

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

vec3 ApplyLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{			
	if (light.type == 0)
	{
	    float distance    = length(light.position - fragPos);		
		vec3 lightDir = normalize(light.position - fragPos);
		// diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
		// specular shading
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// attenuation	
		float attenuation = 1.0 / ((1.0 + 4.5/light.range * distance + 75.0/(light.range*light.range)* (distance * distance)));    
		// combine results
		vec3 ambient  = light.ambient * material.ambient;
		vec3 diffuse  = light.diffuse  * diff * material.diffuse;
		vec3 specular = light.specular * spec * material.specular;	
		if (distance < light.range)
		{
			ambient  *= light.intensity;
			diffuse  *= light.intensity;
			specular *= light.intensity;
		}

		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;	
	

		return (ambient + diffuse + specular);
	}

	else if (light.type == 1)
	{
		vec3 lightDir = normalize(-light.direction);
		//diffuse calculation
		float diff = max(dot(normal,lightDir),0.0);
		//specular
		vec3 reflectDir = reflect(-lightDir,normal);
		float specFactor = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);

		vec3 ambient = light.ambient * material.ambient;
		vec3 diffuse = light.diffuse * diff * material.diffuse;
		vec3 specular = light.specular * specFactor * material.specular;

		ambient  *= light.intensity;
		diffuse  *= light.intensity;
		specular *= light.intensity;

		return (ambient+diffuse+specular); 
	}

	else if (light.type == 2)
	{

		vec3 lightDir = normalize(light.position - fragPos);
		float theta = dot(lightDir, normalize(-light.direction));

			if(theta > light.cutOff)
			{		
				// diffuse shading
				float diff = max(dot(normal, lightDir), 0.0);
				// specular shading
				vec3 reflectDir = reflect(-lightDir, normal);
				float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
				// attenuation	
				float distance    = length(light.position - fragPos);	
				float attenuation = 1.0 / ((1.0 + 4.5/light.range * distance + 75.0/(light.range*light.range)* (distance * distance)));    
				// combine results
				vec3 ambient  = light.ambient * material.ambient;
				vec3 diffuse  = light.diffuse  * diff * material.diffuse;
				vec3 specular = light.specular * spec * material.specular;	

				ambient *=  light.intensity;
				diffuse  *= light.intensity;
				specular *= light.intensity;
				
				ambient *= attenuation;
				diffuse  *=	attenuation;
				specular *=	attenuation;

				return (ambient+diffuse+specular);
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

void main()
{	
	//Point = 0, Directional 1, Spot 2
	vec3 normalizedNormal = normalize(v_normal);
	vec3 normalizedViewDir = normalize(u_cameraPos - v_FragPos);
	
	 vec4 texColor = texture2D(u_texture,v_uv);

	 vec3 finalResult = vec3(0);	
	 for (int i=0; i<MAX_LIGHTS;i++)
	 {
	 	 finalResult += ApplyLight(lights[i],normalizedNormal,v_FragPos,normalizedViewDir);
	 }
	
	gl_FragColor = vec4(finalResult*vec3(texColor),1.0);		
}