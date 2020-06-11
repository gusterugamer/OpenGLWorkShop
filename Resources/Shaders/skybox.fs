precision mediump float;

varying vec3 v_coord;
uniform samplerCube u_cube_texture;
uniform vec3 u_fogColor;

const float lowerLimit = -10.0;
const float upperLimit = 30.0;

void main()
{	
	vec4 finalColor=textureCube(u_cube_texture, v_coord);

	float factor = (v_coord.y-lowerLimit)/(upperLimit-lowerLimit);
	factor = clamp(factor, 0.0,1.0);
	gl_FragColor = mix(vec4(u_fogColor,1.0), finalColor, factor);
}
