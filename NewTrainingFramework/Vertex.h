#pragma once
#include "../Utilities/glm/glm.hpp"
#include <iostream>

struct Vertex 
{
	glm::vec3 pos;	
	glm::vec3 norm;
	glm::vec3 binorm;
	glm::vec3 tgt;
	glm::vec2 uv;
	glm::vec2 uvblend = { 0.0f, 0.0f };
	glm::vec3 color = { 1.0f,0.0f,0.0f };	
};