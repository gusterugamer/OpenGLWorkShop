#pragma once
#include "SceneObject.h"
#include <chrono>

struct FireProperties : SceneObjectProperties
{
	std::vector<int> textureId;
	float dispMax = 0;
}; 

class Fire : public SceneObject
{
public:
	Fire(FireProperties& sop);
	~Fire() override;

	void Draw() override;	
	void Update(ESContext* esContext, const float& deltaTime) override;

private:
	FireProperties sop;

	std::vector<std::shared_ptr<Texture>> textures;	

	unsigned int nrIndicies = 0;
	float u_Time = 0;

	std::chrono::time_point<std::chrono::steady_clock> timeNow;
	std::chrono::time_point<std::chrono::steady_clock> endTime;
};