#include "stdafx.h"
#include "Fire.h"
#include "Renderer.h"
#include "../Utilities/glm/gtc/quaternion.hpp"
#include "../Utilities/glm/gtx/quaternion.hpp"

Fire::Fire(FireProperties & sop)
	:sop(sop)
{
	pMdl = ResourceManager::GetInstance()->loadModel(sop.modelId);
	pShader = ResourceManager::GetInstance()->loadShader(sop.shaderId);
	for (auto texture : sop.textureId)
	{
		textures.push_back(ResourceManager::GetInstance()->loadTexture(texture));
	}	
	pTex = nullptr;
	timeNow = std::chrono::high_resolution_clock::now();
}

Fire::~Fire()
{
}

void Fire::Draw()
{
	glm::mat4 RM;
	glm::mat4 SM;
	glm::mat4 TM;
	glm::mat4 modelMatrix;
	
	RM = glm::toMat4(glm::quat(glm::radians(sop.rotation)));
	SM = glm::scale(glm::mat4(1.0f), sop.scale);
	TM = glm::translate(glm::mat4(1.0f), sop.translation);

	modelMatrix = TM * RM * SM;

	Renderer::DrawMultiTexture(modelMatrix,*pMdl, *pShader, textures);	

	pShader->AddUniform1f("u_Time", u_Time);
	pShader->AddUniform1f("u_dispMax", sop.dispMax);	
}

void Fire::Update(ESContext* esContext, const float & deltaTime)
{	
	//Calculare uniform u_Time
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = endTime - timeNow;
	u_Time = duration.count();
	u_Time = (u_Time - std::floor(u_Time));	
}
