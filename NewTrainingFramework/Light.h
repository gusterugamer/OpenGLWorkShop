#pragma once
#include "../Utilities/Math.h"
#include "SceneObject.h"
#include "Resources.h"


class Light : public SceneObject {
public:
	Light() = default;
	Light(SceneObjectProperties& sop, LightProperties& lp);
	void ChangeLightType(TypeOfLight type);
	TypeOfLight getType() const noexcept;	

	void Draw() override;
	void Update(ESContext* esContext, const float& deltaTime) override;

private:
	LightProperties lp;
	SceneObjectProperties sop;	

	std::vector<Vertex> vert;
	std::vector<GLushort> indicies;

	IndexBuffer ib;
	VertexBuffer vb;

	Model model;

	const float quadDimension = 5.0f;

private:
	void BuildQuad();	
};