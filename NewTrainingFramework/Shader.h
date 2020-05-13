#pragma once
#include "Resources.h"
#include "../Utilities/esUtil.h"
#include <memory>
class Shader
{
protected:	
	 std::shared_ptr<ShaderResource> sr;

	GLuint program, vertexShader, fragmentShader;	

public:
	Shader(std::shared_ptr<ShaderResource> sr);
	~Shader();

	virtual void Load();

	GLint AddAttrib(const char* name);

	GLint AddUniform(const char * name);	

	GLint Shader::GetProgramId();

	GLint positionAttribute;	
	GLint textureCoordAttrib;
	GLint normalAttrib;

	GLint textureUniform;	
	GLint viewMatrixUniform;
	GLint modelMatrixUniform;
	GLint perspectiveMatrixUniform;
};