#pragma once
#include "Resources.h"
#include "../Utilities/esUtil.h"
#include "Texture.h"
#include <memory>
#include <unordered_map>

class Shader
{
private:	
	 std::shared_ptr<ShaderResource> sr;

	GLuint program, vertexShader, fragmentShader;
	std::unordered_map<std::string, int> locationCache;

private:
	GLint getLocation(const std::string& name);

public:
	Shader(std::shared_ptr<ShaderResource> sr);
	~Shader();

	void Load();

	//Attributes

	void AddAttrib(const char* name, const int& size, const int& type, const bool& normalized, const int& stride, const void* ptr);

	//Unforms

	//Float
	
	void AddUniform1f(const char* name, float value);
	void AddUniform2f(const char* name, float value1, float value2);
	void AddUniform2f(const char* name, const glm::vec2& vec);
	void AddUniform3f(const char* name, float value1, float value2, float value3);	
	void AddUniform3f(const char* name, const glm::vec3& vec);
	void AddUniformInArray1f(const char* arrayName, const int& index, const char* propertyName, float value);
	void AddUniformInArray2f(const char* arrayName, const int& index, float value1, const char* propertyName,float value2);
	void AddUniformInArray2f(const char* arrayName, const int& index, const char* propertyName,  const glm::vec2& vec);
	void AddUniformInArray3f(const char* arrayName, const int& index, const char* propertyName, float value1, float value2, float value3);
	void AddUniformInArray3f(const char* arrayName, const int& index, const char* propertyName, const glm::vec3& vec);

	//Integer
	void AddUniform1i(const char* name, int value);
	void AddUniform2i(const char* name, int value1, int value2);
	//TODO: Add functions for glm::vec2 of ints
	void AddUniform3i(const char* name, int value1, int value2, int value3);
	//TODO: Add functions for glm::vec3 of ints
	void AddUniformInArray1i(const char* arrayName, const int& index, const char* propertyName, int value);
	void AddUniformInArray2i(const char* arrayName, const int& index, const char* propertyName, int value1, int value2);
	//TODO: Add functions for glm::vec2 of ints
	void AddUniformInArray3i(const char* arrayName, const int& index, const char* propertyName, int value1, int value2, int value3);
	//TODO: Add functions for glm::vec3 of ints

	//glm::mat4 
	void AddMatrix2fv(const char* name, int count, bool transpose,const glm::mat2& mat);
	void AddMatrix3fv(const char* name, int count, bool transpose,const glm::mat3& mat);
	void AddMatrix4fv(const char* name, int count, bool transpose,const glm::mat4& mat);

	//Texture
	void AddTexture(const char* uniformName, const Texture& tex);
	void AddMultiTexture(const char* uniformName, const int& index, const std::shared_ptr<Texture>& tex);

	GLint Shader::GetProgramId();
};