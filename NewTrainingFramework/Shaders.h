#pragma once
#include "../Utilities/utilities.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];

	GLint positionAttribute;
	GLint colorAttribute;
	GLint textureAttribute;

	GLint textureUniform;
	GLint rotationUniform;	
	GLint viewMatrixUniform;
	GLint modelMatrixUniform;
	GLint perspectiveMatrixUniform;
	


	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};