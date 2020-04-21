#include "stdafx.h"
#include "Skybox.h"
#include "OpenGLErrorChecking.h"
#include "SceneManager.h"

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
	
	Renderer::DrawSkyBox(pMdl->GetVd(), pMdl->GetIb(), *pShader, *pTex);

	//Trasmitere modelMatrix catre fragment shader	
	GLCall(glUniformMatrix4fv(pShader->modelMatrixUniform, 1, GL_FALSE, (GLfloat*)modelMatrix.m));	
}

void Skybox::Update(ESContext* esContext,const float & deltaTime)
{
	Vector3 CameraPos = SceneManager::GetInstance()->GetCurrentCamera()->GetPosition();
	sop.translation = CameraPos;
}
