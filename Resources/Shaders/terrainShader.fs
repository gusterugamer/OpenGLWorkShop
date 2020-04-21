precision mediump float;

varying vec2 v_uv;
varying vec2 v_uvblend;

uniform sampler2D u_texture[4];
uniform float u_moveTexX;
uniform float u_moveTexY;

void main()
{	
	vec4 dirt = texture2D(u_texture[0], v_uv);	
	vec4 rock = texture2D(u_texture[1], v_uv);
	vec4 grass = texture2D(u_texture[2], v_uv);
	vec2 vblend = vec2(v_uvblend.x+u_moveTexX, v_uvblend.y+u_moveTexY);
	vec4 blendMapColor = texture2D(u_texture[3], vblend);

	vec4 terrTex = (blendMapColor.r * rock + blendMapColor.g*grass+blendMapColor.b*dirt);
	
	gl_FragColor = terrTex;
}
