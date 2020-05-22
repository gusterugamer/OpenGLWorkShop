attribute vec3 a_pos;
attribute vec3 a_color;
attribute vec2 a_uv;

uniform mat4 u_LightMatrix;
uniform mat4 u_perspectiveMatrix;

varying vec2 v_uv;
varying vec3 v_color;

void main()
{	
	gl_Position = u_perspectiveMatrix * u_LightMatrix * vec4(a_pos,1.0);	
	v_uv = a_uv;	
	v_color = a_color;
}
   