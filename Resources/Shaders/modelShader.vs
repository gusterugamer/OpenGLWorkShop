attribute vec3 a_pos;
attribute vec2 a_uv;
attribute vec3 a_normal;

varying vec2 v_uv;
varying vec3 v_normal;
varying vec3 v_toLightVec;
varying vec3 v_toCameraVec;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_perspective;
uniform vec3 u_lightPosition;
uniform vec3 u_cameraPos;

void main()
{	
	vec4 pos = vec4(a_pos, 1.0);
	vec4 worldPosition = u_model * pos;	

	v_uv = a_uv;
	v_normal = vec3(u_model*vec4(a_normal,0.0));
	v_toLightVec = u_lightPosition - vec3(worldPosition);

	gl_Position = u_perspective * u_view *u_model * pos;	
	v_toCameraVec = u_cameraPos - vec3(worldPosition);
}
   