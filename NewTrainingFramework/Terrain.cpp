#include "stdafx.h"
#include "Terrain.h"
#include "DebugModeFunctions.h"
#include "SceneManager.h"
#include "Renderer.h"

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

	//Calculare model matrix teren;
	RM = RMx.SetRotationX(sop.rotation.x) * RMy.SetRotationY(sop.rotation.y) *
		RMz.SetRotationZ(sop.rotation.z);
	SM = SM.SetScale(sop.scale);
	TM = TM.SetTranslation(sop.translation);

	modelMatrix = SM * RM * TM;

	Renderer::DrawBlendedTextures(modelMatrix,vb,ib,*pShader,terrainTexMap);

	//Trasmitere modelMatrix catre fragment shader	
	
	/*GLint uniformMoveTexX = pShader->AddUniform("u_moveTexX");
	GLint uniformMoveTexY = pShader->AddUniform("u_moveTexY");
	GLint uniformHeight = pShader->AddUniform("u_height");

	GLCall(glUniform1f(uniformMoveTexX, moveTextureX));
	GLCall(glUniform1f(uniformMoveTexY, moveTextureY));	
	GLCall(glUniform3f(uniformHeight, sop.height.x, sop.height.y, sop.height.z));*/
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
			v.pos = Vector3(
				((x - (float)std::ceil(sop.numarCelule / 2))*sop.dimensiuneCelule) - (float)sop.dimensiuneCelule / 2,
				-sop.offSetY,
				-((z - (float)std::ceil(sop.numarCelule / 2))*sop.dimensiuneCelule + (float)sop.dimensiuneCelule / 2)
			);


			//Generare UV celule
			v.uv = Vector2(x, z);

			//Generare UV blend texture
			v.uvblend = Vector2(x / sop.numarCelule, z / sop.numarCelule);
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