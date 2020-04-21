precision mediump float;

varying vec3 v_coord;
uniform samplerCube u_cube_texture;

void main()
{	
	gl_FragColor=textureCube(u_cube_texture, v_coord);
}
