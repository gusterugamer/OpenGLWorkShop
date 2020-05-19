#pragma once
#include "../Utilities/Mathematics.h"
#include <iostream>

struct Vertex 
{
	Vector3 pos;	
	Vector3 norm;
	Vector3 binorm;
	Vector3 tgt;
	Vector2 uv;
	Vector2 uvblend = { 0.0f, 0.0f };
	Vector3 color = { 1.0f,0.0f,0.0f };	
};