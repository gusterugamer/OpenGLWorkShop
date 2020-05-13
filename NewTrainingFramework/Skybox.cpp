#include "stdafx.h"
#include "Skybox.h"
#include "DebugModeFunctions.h"
#include "SceneManager.h"
#include "Renderer.h"

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
	RM = RMx.SetRotationX(sop.rotation.x) * RMy.SetRotationY(sop.rotation.y) *
		RMz.SetRotationZ(sop.rotation.z);
	SM = SM.SetScale(sop.scale);
	TM = TM.SetTranslation(sop.translation);

	modelMatrix = SM*  RM * TM;
	
	Renderer::DrawSkyBox(modelMatrix,*pMdl, *pShader, *pTex);	
}

void Skybox::Update(ESContext* esContext,const float & deltaTime)
{
	Vector3 CameraPos = SceneManager::GetInstance()->GetCurrentCamera()->GetPosition();
	sop.translation = CameraPos;
}
