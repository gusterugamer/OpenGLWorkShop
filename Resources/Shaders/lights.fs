precision mediump float;

varying vec2 v_uv;
varying vec3 v_color;

uniform sampler2D u_LightTexture;

void main()
{	
	vec4 color = texture2D(u_LightTexture,v_uv);		
	gl_FragColor = color;
}
