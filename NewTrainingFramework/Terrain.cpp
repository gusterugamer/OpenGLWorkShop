#include "stdafx.h"
#include "Terrain.h"
#include "DebugModeFunctions.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "../Utilities/glm/gtc/quaternion.hpp"
#include "../Utilities/glm/gtx/quaternion.hpp"

Terrain::Terrain(TerrainProperties& sop)
	:sop(sop)
{	
	pMdl = nullptr;
	pShader = ResourceManager::GetInstance()->loadShader(sop.shaderId);
	Init();	
}

Terrain::~Terrain()
{
	vb.DeleteBuffer();
	ib.DeleteBuffer();
}

void Terrain::Draw()
{

	//Calculare model glm::mat4 teren;
	glm::mat4 RM;
	glm::mat4 SM;
	glm::mat4 TM;
	glm::mat4 modelMatrix;

	glm::quat quaternion = glm::quat(sop.rotation);
	RM = glm::toMat4(quaternion);
	SM = glm::scale(glm::mat4(1.0f), sop.scale);
	TM = glm::translate(glm::mat4(1.0f), sop.translation);

	modelMatrix = TM * RM * SM;

	Renderer::DrawBlendedTextures(modelMatrix,vb,ib,*pShader,terrainTexMap);

	//Trasmitere modelMatrix catre fragment shader	
	

	//pShader->AddUniform1f("u_moveTexX", moveTextureX);
	//pShader->AddUniform1f("u_moveTexY", moveTextureY);
}

void Terrain::Init(){

	//Generare teren
	vert.reserve((sop.numarCelule + 1)*(sop.numarCelule + 1));

	//Generare pozitii vertecsi teren( culoare este rosie implicit) si UV pentru celule
	for (float z = 0; z <= sop.numarCelule; z++)
		for (float x = 0; x <= sop.numarCelule; x++)
		{
			Vertex v;
			//Centrare teren la pozitia camerei
			v.pos = glm::vec3(
				((x - (float)std::ceil(sop.numarCelule / 2))*sop.dimensiuneCelule) - (float)sop.dimensiuneCelule / 2,
				-sop.offSetY,
				-((z - (float)std::ceil(sop.numarCelule / 2))*sop.dimensiuneCelule + (float)sop.dimensiuneCelule / 2)
			);


			//Generare UV celule
			v.uv = glm::vec2(x, z);

			//Generare UV blend texture
			v.uvblend = glm::vec2(x / sop.numarCelule, z / sop.numarCelule);
			vert.push_back(v);
		}

	//Generare indici teren
	for (int i = 0; i < sop.numarCelule; i++)
		for (int j = 0; j < sop.numarCelule; j++)
		{
			//Generare indici teren
			indicies.push_back((i + 1)*(sop.numarCelule + 1) + j);
			indicies.push_back(i*(sop.numarCelule + 1) + j);
			indicies.push_back((i*(sop.numarCelule + 1) + j + 1));
			indicies.push_back((i + 1)*(sop.numarCelule + 1) + j);
			indicies.push_back(i*(sop.numarCelule + 1) + j + 1);
			indicies.push_back((i + 1)*(sop.numarCelule + 1) + j + 1);
		}

	nrIndicies = indicies.size();

	//Initializare buffere pentru teren
	vb = { vert.data(),vert.size() };
	ib = { indicies.data(),nrIndicies };

	//Incarcare texturi	
	terrainTexMap.reserve(sop.textureId.size());
	for (auto texture : sop.textureId)	
		terrainTexMap.push_back(ResourceManager::GetInstance()->loadTexture(texture));	
}

void Terrain::Update(ESContext* esContext,const float& deltaTime)
{	
	dx = SceneManager::GetInstance()->GetCurrentCamera()->GetPosition().x - sop.translation.x;
	dz = SceneManager::GetInstance()->GetCurrentCamera()->GetPosition().z - sop.translation.z;

	if (std::abs(dx) >= sop.dimensiuneCelule && dx>0)
	{
		moveTextureX += (1 / (float)sop.numarCelule);
		sop.translation.x += std::abs(dx);
	}
	if (std::abs(dx) >= sop.dimensiuneCelule && dx < 0)
	{
		moveTextureX -= (1 / (float)sop.numarCelule);
		sop.translation.x -= std::abs(dx);
	}
	if (std::abs(dz) >= sop.dimensiuneCelule && dz > 0)
	{
		moveTextureY -= (1 / (float)sop.numarCelule);
		sop.translation.z += std::abs(dz);
	}
	if (std::abs(dz) >= sop.dimensiuneCelule && dz < 0)
	{
		moveTextureY += (1 / (float)sop.numarCelule);
		sop.translation.z -= std::abs(dz);		
	}	
}