precision mediump float;

varying vec2 v_uv;
uniform sampler2D u_texture;

vec4 modelTexture;
void main()
{	
	vec4 texColor = texture2D(u_texture, v_uv);		
	if (texColor.a <= 0.1)
		discard;
	gl_FragColor = texColor;	
}
