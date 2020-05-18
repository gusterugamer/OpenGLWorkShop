#include "stdafx.h"
#include "Shader.h"
#include "DebugModeFunctions.h"
#include <iostream>
#include <chrono>

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
	vertexShader = esLoadShader(GL_VERTEX_SHADER, sr->vs.c_str());

	if (vertexShader == 0)
	{
		std::cout << "Vertex Shader was not loaded" <<std::endl;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, sr->fs.c_str());

	if (fragmentShader == 0)
	{
		std::cout << "Fragment shader was not loaded" << std::endl;
	}

	program = esLoadProgram(vertexShader, fragmentShader);	
}

void Shader::AddAttrib(const char* name, const int& size, const int& type, const bool& normalized,const int& stride, const void* ptr)
{
	GLint loc = glGetAttribLocation(program, name);
	GLCall(glEnableVertexAttribArray(loc));
	GLCall(glVertexAttribPointer(loc, size, type, normalized,stride,ptr));	
}

//Floats
void Shader::AddUniform1f(const char* name, float value)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform1f(loc, value));
}

void Shader::AddUniform2f(const char* name, float value1, float value2)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform2f(loc, value1, value2));
}

void Shader::AddUniform2f(const char* name, const Vector2& vec)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform2f(loc, vec.x, vec.y));
}

void Shader::AddUniform3f(const char* name, float value1, float value2, float value3)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform3f(loc, value1, value2, value3));
}

void Shader::AddUniform3f(const char* name, const Vector3& vec)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform3f(loc, vec.x, vec.y, vec.z));
}

void Shader::AddUniformInArray1f(const char* arrayName, const int& index, const char* propertyName, float value)
{	
	char name[255];	
	strcpy_s(name, arrayName);
	strcat_s(name, "[");
	strcat_s(name, std::to_string(index).c_str());
	strcat_s(name, "].");
	strcat_s(name, propertyName);

	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform1f(loc, value));	
}

void Shader::AddUniformInArray2f(const char* arrayName, const int& index, float value1, const char* propertyName, float value2)
{
	char name[255];
	strcpy_s(name, arrayName);
	strcat_s(name, "[");
	strcat_s(name, std::to_string(index).c_str());
	strcat_s(name, "].");
	strcat_s(name, propertyName);

	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform2f(loc, value1,value2));
}

void Shader::AddUniformInArray2f(const char* arrayName, const int& index, const char* propertyName, const Vector2& vec)
{
	char name[255];
	strcpy_s(name, arrayName);
	strcat_s(name, "[");
	strcat_s(name, std::to_string(index).c_str());
	strcat_s(name, "].");
	strcat_s(name, propertyName);

	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform2f(loc, vec.x, vec.y));
}

void Shader::AddUniformInArray3f(const char* arrayName, const int& index, const char* propertyName, float value1, float value2, float value3)
{
	char name[255];
	strcpy_s(name, arrayName);
	strcat_s(name, "[");
	strcat_s(name, std::to_string(index).c_str());
	strcat_s(name, "].");
	strcat_s(name, propertyName);

	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform3f(loc, value1, value2, value3));
}

void Shader::AddUniformInArray3f(const char* arrayName, const int& index, const char* propertyName, const Vector3& vec)
{
	char name[255];
	strcpy_s(name, arrayName);
	strcat_s(name, "[");
	strcat_s(name, std::to_string(index).c_str());
	strcat_s(name, "].");
	strcat_s(name, propertyName);

	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform3f(loc, vec.x, vec.y, vec.z));
}

//Integers
void Shader::AddUniform1i(const char* name, int value)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform1i(loc, value));
}

void Shader::AddUniform2i(const char* name, int value1, int value2)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform2i(loc, value1, value2));
}

void Shader::AddUniform3i(const char* name, int value1, int value2, int value3)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform3i(loc, value1, value2, value3));
}

void Shader::AddUniformInArray1i(const char* arrayName, const int& index, const char* propertyName, int value)
{
	char name[255];
	strcpy_s(name, arrayName);
	strcat_s(name, "[");
	strcat_s(name, std::to_string(index).c_str());
	strcat_s(name, "].");
	strcat_s(name, propertyName);

	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform1i(loc, value));
}

void Shader::AddUniformInArray2i(const char* arrayName, const int& index, const char* propertyName, int value1, int value2)
{
	char name[255];
	strcpy_s(name, arrayName);
	strcat_s(name, "[");
	strcat_s(name, std::to_string(index).c_str());
	strcat_s(name, "].");
	strcat_s(name, propertyName);

	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform2i(loc, value1, value2));
}

void Shader::AddUniformInArray3i(const char* arrayName, const int& index, const char* propertyName, int value1, int value2, int value3)
{
	char name[255];
	strcpy_s(name, arrayName);
	strcat_s(name, "[");
	strcat_s(name, std::to_string(index).c_str());
	strcat_s(name, "].");
	strcat_s(name, propertyName);

	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniform3i(loc, value1, value2, value3));
}

//Matrix

void Shader::AddMatrix2fv(const char* name, int count, bool transpose,const Matrix& mat)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniformMatrix4fv(loc, count, transpose, (GLfloat*)mat.m));	
}
																	  
void Shader::AddMatrix3fv(const char* name, int count, bool transpose,const Matrix& mat)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniformMatrix4fv(loc, count, transpose, (GLfloat*)mat.m));
}
																	 
void Shader::AddMatrix4fv(const char* name, int count, bool transpose,const Matrix& mat)
{
	GLint loc = glGetUniformLocation(program, name);
	GLCall(glUniformMatrix4fv(loc, count, transpose, (GLfloat*)mat.m));
}

//Texture

void Shader::AddTexture(const char* uniformName, const Texture& tex)
{
	GLint loc = glGetUniformLocation(program, uniformName);
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glUniform1i(loc, 0));
	tex.Bind();
}

void Shader::AddMultiTexture(const char* uniformName, const int& index, const std::shared_ptr<Texture>& tex)
{
	char name[255];
	strcpy_s(name, uniformName);
	strcat_s(name, "[");
	strcat_s(name, std::to_string(index).c_str());
	strcat_s(name, "]");	

	GLint loc = glGetUniformLocation(program, name);
	GLCall(glActiveTexture(GL_TEXTURE0 + index));
	GLCall(glUniform1i(loc, 0+index));
	tex->Bind();
}

GLint Shader::GetProgramId()
{
	return program;
}
