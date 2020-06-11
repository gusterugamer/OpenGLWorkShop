attribute vec3 a_pos;
attribute vec2 a_uv;
attribute vec3 a_normal;
attribute vec3 a_binorm;
attribute vec3 a_tan;

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_FragPos;
varying float v_visibility;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_perspective;
uniform vec3 u_lightPosition;

const float density = 0.007;
const float gradient = 5.0;

void main()
{	
	vec4 pos = vec4(a_pos, 1.0);
	vec4 worldPosition = u_model * pos;
	vec4 positionRelativeToCam = u_view * worldPosition;
	
	v_FragPos = worldPosition.xyz;
	v_uv = a_uv;
	v_normal = a_normal;

	gl_Position = u_perspective * positionRelativeToCam;
	
	float distance = length(positionRelativeToCam.xyz);
	v_visibility  = exp(-pow((distance*density),gradient));
	v_visibility = clamp(v_visibility, 0.0, 1.0);
}
   