attribute vec3 a_pos;
attribute vec3 a_color;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_perspective;

varying vec3 v_coord;

void main()
{	
	vec4 pos = vec4(a_pos, 1.0);	
	gl_Position = u_perspective * u_view *  u_model * pos;
	v_coord = a_pos;
}
   