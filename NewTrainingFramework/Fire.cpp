#include "stdafx.h"
#include "Fire.h"
#include "OpenGLErrorChecking.h"

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
	RM = RMx.SetRotationX(sop.rotation.x) * RMy.SetRotationY(sop.rotation.y) * RMz.SetRotationZ(sop.rotation.z);
	SM = SM.SetScale(sop.scale);
	TM = TM.SetTranslation(sop.translation);

	modelMatrix = SM * RM * TM;

	GLint unifTime = pShader->AddUniform("u_Time");
	GLint unifDispMax = pShader->AddUniform("u_dispMax");
	Renderer::DrawTerrain(pMdl->GetVd(), pMdl->GetIb(), *pShader, textures);		

	GLCall(glUniformMatrix4fv(pShader->modelMatrixUniform, 1, GL_FALSE, (GLfloat*)modelMatrix.m));
	GLCall(glUniform1f(unifTime, u_Time));
	GLCall(glUniform1f(unifDispMax, sop.dispMax));

	GLCall(glDisable(GL_BLEND));
}

void Fire::Update(ESContext* esContext, const float & deltaTime)
{	
	//Calculare uniform u_Time
	endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = endTime - timeNow;
	u_Time = duration.count();
	u_Time = (u_Time - std::floor(u_Time));	
}
