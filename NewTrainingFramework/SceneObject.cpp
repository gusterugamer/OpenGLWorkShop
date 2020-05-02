#include "stdafx.h"
#include <string>
#include <iostream>
#include "OpenGLErrorChecking.h"
#include "SceneManager.h"
#include "Renderer.h"

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
	light = Light({ Vector3(0.0f,250.0f,0.0f),Vector3(1.0f,1.0f,1.0f),Vector3(1.0f,0.0f,0.0f),TypeOfLight::Point });
	Renderer::Draw(sop,*pMdl,*pShader,*pTex,light);	
}

void SceneObject::Update(ESContext* esContext, const float& deltaTime)
{
}
