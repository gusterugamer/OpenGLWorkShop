#include "stdafx.h"
#include "Shader.h"
#include <iostream>

Shader::Shader(std::shared_ptr<ShaderResource> sr)
{
	this->sr = sr;
}

Shader::~Shader()
{	
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Load()
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, const_cast<char*>(sr->vs.c_str()));

	if (vertexShader == 0)
	{
		std::cout << "Vertex Shader was not loaded" <<std::endl;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, const_cast<char*>(sr->fs.c_str()));

	if (fragmentShader == 0)
	{
		std::cout << "Fragment shader was not loaded" << std::endl;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//Atributes
	positionAttribute = glGetAttribLocation(program, "a_pos");		
	textureCoordAttrib = glGetAttribLocation(program, "a_uv");	

	//Uniforms
	textureUniform = glGetUniformLocation(program, "u_texture");
	viewMatrixUniform = glGetUniformLocation(program, "u_view");
	modelMatrixUniform = glGetUniformLocation(program, "u_model");
	perspectiveMatrixUniform = glGetUniformLocation(program, "u_perspective");
}

GLint Shader::AddAttrib(const char* name)
{
	return glGetAttribLocation(program, name);
}

GLint Shader::AddUniform(const char* name)
{
	return glGetUniformLocation(program, name);
}

GLint Shader::GetProgramId()
{
	return program;
}
