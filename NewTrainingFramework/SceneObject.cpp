#include "stdafx.h"
#include <string>
#include <iostream>
#include "DebugModeFunctions.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Light.h"

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
	Renderer::Draw(sop, *pMdl, *pShader, *pTex);
}

void SceneObject::Update(ESContext* esContext, const float& deltaTime)
{
}

void SceneObject::FixedUpdate(ESContext* esContext, const float& fixedDeltaTime)
{
}
