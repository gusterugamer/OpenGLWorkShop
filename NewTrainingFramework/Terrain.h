#pragma once
#include "SceneObject.h"
#include "Shader.h"
#include <string>
#include "Renderer.h"

struct TerrainProperties : SceneObjectProperties
{
	std::vector<int> textureId;
	float offSetY;
	int numarCelule;
	int dimensiuneCelule;
	Vector3 height;
};

class Terrain : public SceneObject
{
public:
	Terrain(TerrainProperties& sop);
	~Terrain() override; 

	void Draw() override;
	void Init();	
	void Update(ESContext* esContext, const float& deltaTime) override;

private:
	TerrainProperties sop;

	std::vector<std::shared_ptr<Texture>> terrainTexMap;

	GLuint vboId;
	GLuint vboIdBlend;
	GLuint iboID;

	float moveTextureX = 0.0f;
	float moveTextureY = 0.0f;
	
	std::vector<Vertex> vert;
	std::vector<GLushort> indicies;

	unsigned int nrIndicies = 0;
	float dx = 0;
	float dz = 0;

	IndexBuffer ib;
	VertexBuffer vb;

	/////////////////////TEMPORRAR///////////////////////////
	
	////////////////////TEMPORAR////////////////////////////
};


