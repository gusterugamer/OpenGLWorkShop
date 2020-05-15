#include "stdafx.h"
#include "Light.h"
#include "DebugModeFunctions.h"
#include "Renderer.h"

Light::Light(SceneObjectProperties& sop, LightProperties& lp)
	:
	sop(sop),
	lp(lp)
{	
	pMdl = nullptr;
	pTex = ResourceManager::GetInstance()->loadTexture(sop.textureId);
	pShader = ResourceManager::GetInstance()->loadShader(sop.shaderId);
	BuildQuad();	
}

void Light::ChangeLightType(TypeOfLight type)
{
	//switch (type)
	//{
	//case TypeOfLight::Point:
	//{
	//	if (this->type != type)
	//	{
	//		PointLightProperties tempSop;
	//		tempSop.direction = sop.direction;
	//		tempSop.position = sop.position;
	//		tempSop.colorAmb = sop.colorAmb;
	//		tempSop.colorDif = sop.colorDif;
	//		tempSop.colorSpec = sop.colorSpec;
	//		sop = tempSop;
	//		//Todo: Change light symbol for when added
	//	}
	//}
	//break;

	//case TypeOfLight::Directional:
	//{
	//	if (this->type != type)
	//	{
	//		LightProperties tempSop;
	//		tempSop.direction = sop.direction;
	//		tempSop.position = sop.position;
	//		tempSop.colorAmb = sop.colorAmb;
	//		tempSop.colorDif = sop.colorDif;
	//		tempSop.colorSpec = sop.colorSpec;
	//		sop = tempSop;
	//		//Todo: Change light symbol for when added
	//	}
	//}
	//break;

	//case TypeOfLight::Spot:
	//{
	//	if (this->type != type)
	//	{
	//		SpotLightProperties tempSop;
	//		tempSop.direction = sop.direction;
	//		tempSop.position = sop.position;
	//		tempSop.colorAmb = sop.colorAmb;
	//		tempSop.colorDif = sop.colorDif;
	//		tempSop.colorSpec = sop.colorSpec;
	//		sop = tempSop;
	//		//Todo: Change light symbol for when added
	//	}
	//}
	//break;

	//}
}

TypeOfLight Light::getType() const noexcept
{
	return lp.type;
}

void Light::Draw()
{
	Renderer::DrawDebug(sop,vb,ib, *pShader, *pTex);
}

void Light::Update(ESContext* esContext, const float& deltaTime)
{
}

void Light::BuildQuad()
{	
	for (float y = 1; y < 3; y++)
		for (float x = 1; x < 3; x++)
		{
			Vertex v;
			v.pos = Vector3(
				(x  + (float)std::pow(-1,x) *(float)quadDimension/2),
				(y  + (float)std::pow(-1,y) *(float)quadDimension/2),
				0
			);
			v.uv = Vector2(x-1, y-1);
			v.color = Vector3(1.0, 1.0, 1.0);
			vert.push_back(v);
			
		}

	indicies.push_back(0);
	indicies.push_back(1);
	indicies.push_back(2);
	indicies.push_back(1);
	indicies.push_back(2);
	indicies.push_back(3);

	vb = { vert.data(),vert.size() };
	ib = { indicies.data(),indicies.size() };	
}
