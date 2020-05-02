#pragma once
#include "SceneObject.h"

struct SkyBoxProps : SceneObjectProperties {	
};

class Skybox : public SceneObject
{
public:
	Skybox(SkyBoxProps& sop);
	~Skybox() override;

	void Draw() override;
	void Update(ESContext* esContext,const float& deltaTime) override;
private:

	SkyBoxProps sop;
};