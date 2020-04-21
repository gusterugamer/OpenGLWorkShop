#include <stdafx.h>
#include "Shaders.h"


int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	colorAttribute  = glGetAttribLocation(program, "a_color");	
	textureAttribute = glGetAttribLocation(program, "a_uv");

	rotationUniform = glGetUniformLocation(program, "u_rot");
	textureUniform = glGetUniformLocation(program, "u_texture");
	viewMatrixUniform = glGetUniformLocation(program, "u_View");
	modelMatrixUniform = glGetUniformLocation(program, "u_Model");
	perspectiveMatrixUniform = glGetUniformLocation(program, "u_Perspective");

	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}