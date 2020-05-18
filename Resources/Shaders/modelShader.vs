attribute vec3 a_pos;
attribute vec2 a_uv;
attribute vec3 a_normal;
attribute vec3 a_binorm;
attribute vec3 a_tan;

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_FragPos;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_perspective;
uniform vec3 u_lightPosition;

void main()
{	
	vec4 pos = vec4(a_pos, 1.0);
	vec4 worldPosition = u_model * pos;	
	v_FragPos = worldPosition.xyz;

	v_uv = a_uv;
	v_normal = a_normal;
	
//	 vec3 normalizedNorm=   normalize(a_normal);
//	 vec3 normalizedBinorm=	normalize(a_binorm);
//	 vec3 normalizedTan=	normalize(a_tan);
//	
//	mat3 TBN = mat3(
//	normalizedTan.x, normalizedBinorm.x,normalizedNorm.x, 
//	normalizedTan.y, normalizedBinorm.y,normalizedNorm.y, 
//	normalizedTan.z, normalizedBinorm.z,normalizedNorm.z	
//	);	

	gl_Position = u_perspective * u_view *u_model * pos;		
}
   