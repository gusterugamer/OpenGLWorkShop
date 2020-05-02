precision mediump float;

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_toLightVec;
varying vec3 v_toCameraVec;

uniform sampler2D u_texture;
uniform vec3 u_lightColorDif;
uniform vec3 u_lightColorSpec;
uniform float u_lightType;
uniform float u_shineDamper;
uniform float u_reflectivity;

void main()
{	
	
	vec3 normalizedNormal = normalize(v_normal);
	vec3 normalizedToLightVec = normalize(v_toLightVec);	

	//Calculation for difuseLight	
	float dot1=dot(normalizedNormal, normalizedToLightVec);
	float brightness = max(dot1,0.0);
	vec3 difuseLight = brightness * u_lightColorDif;

	//calculation for specularLight
	vec3 normalizedToCameraVec = normalize(v_toCameraVec);
	vec3 lightDirection = -normalizedToLightVec;
	vec3 reflectedLightDir = reflect(lightDirection,normalizedNormal);

	float specularFactor = dot(reflectedLightDir, normalizedToCameraVec);
	specularFactor = max(specularFactor, 0.0);
	float dampedFactor = pow(specularFactor, u_shineDamper);
	vec3 finalSpecular = dampedFactor * u_reflectivity* u_lightColorSpec;	

	vec4 texColor = vec4(difuseLight ,1.0)* texture2D(u_texture, v_uv) + vec4(finalSpecular,1.0);			
	gl_FragColor = texColor;	
}
