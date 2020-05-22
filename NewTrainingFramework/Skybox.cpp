#include "stdafx.h"
#include "Skybox.h"
#include "DebugModeFunctions.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "../Utilities/glm/gtc/quaternion.hpp"
#include "../Utilities/glm/gtx/quaternion.hpp"

Skybox::Skybox(SkyBoxProps& sop)
	:sop(sop)
{
	pMdl = ResourceManager::GetInstance()->loadModel(sop.modelId);
	pShader = ResourceManager::GetInstance()->loadShader(sop.shaderId);
	pTex = ResourceManager::GetInstance()->loadTexture(sop.textureId);	
}

Skybox::~Skybox()
{
}

void Skybox::Draw()
{	
	glm::mat4 RM;
	glm::mat4 SM;
	glm::mat4 TM;
	glm::mat4 modelMatrix;

	glm::quat quaternion = glm::quat(glm::radians(sop.rotation));
	RM = glm::toMat4(quaternion);
	SM = glm::scale(glm::mat4(1.0f), sop.scale);
	TM = glm::translate(glm::mat4(1.0f), sop.translation);

	modelMatrix = TM * RM * SM;
	
	Renderer::DrawSkyBox(modelMatrix,*pMdl, *pShader, *pTex);	
}

void Skybox::Update(ESContext* esContext,const float & deltaTime)
{
	glm::vec3 CameraPos = SceneManager::GetInstance()->GetCurrentCamera()->GetPosition();
	sop.translation = CameraPos;
}
