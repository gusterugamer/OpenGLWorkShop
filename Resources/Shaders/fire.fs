precision highp float;

varying vec2 v_uv;

uniform sampler2D u_texture[3];
uniform float u_Time;
uniform float u_dispMax;
void main()
{	
	vec2 dispuv = vec2(v_uv.x,v_uv.y+u_Time);
	vec2 offset = ((texture2D(u_texture[1],dispuv).rg)*2.0 - 1.0) * u_dispMax;
	vec2 v_uv_displaced = v_uv+offset;
	vec4 c_fire = texture2D(u_texture[2],v_uv_displaced);
	vec4 c_alpha = texture2D(u_texture[0],v_uv);
	
	gl_FragColor = c_fire * c_alpha.r;
}
