attribute vec3 a_pos;
attribute vec3 a_color;
attribute vec2 a_uv;
attribute vec2 a_uvblend;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_perspective;
uniform vec3 u_height;
uniform sampler2D u_texture[4];

varying vec2 v_uv;
varying vec2 v_uvblend;

void main()
{
	
	vec4 pos = vec4(a_pos, 1.0);
	//vec4 c_blend = texture2D(u_texture[3], a_uvblend);
	//pos.y += c_blend.r*u_height.r+c_blend.g*u_height.g+c_blend.b*u_height.b;
	gl_Position = u_perspective * u_view *  u_model * pos;	
	v_uv = a_uv;
	v_uvblend = a_uvblend;
}
   