#include "stdafx.h"
#include <string>
#include <iostream>
#include "OpenGLErrorChecking.h"
#include "SceneManager.h"

SceneObject::~SceneObject()
{
}

SceneObject::SceneObject(SceneObjectProperties& sop)
	:sop(sop)
{
	pMdl = ResourceManager::GetInstance()->loadModel(sop.modelId);
	pShader = ResourceManager::GetInstance()->loadShader(sop.shaderId);		  
	pTex = ResourceManager::GetInstance()->loadTexture(sop.textureId);		
}

void SceneObject::Draw()
{
	RM = RMx.SetRotationX(sop.rotation.x) * RMy.SetRotationY(sop.rotation.y) *
		RMz.SetRotationZ(sop.rotation.z);
	SM = SM.SetScale(sop.scale);
	TM = TM.SetTranslation(sop.translation);

	modelMatrix = SM*TM*RM;
	Renderer::Draw(pMdl->GetVd(), pMdl->GetIb(), *pShader, *pTex);
	
	GLCall(glUniformMatrix4fv(pShader->modelMatrixUniform, 1, GL_FALSE, (GLfloat*)modelMatrix.m));
	//Trasmitere modelMatrix catre fragment shader		
	
}

void SceneObject::Update(ESContext* esContext, const float& deltaTime)
{		
}
