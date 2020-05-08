#pragma once
#include "../Utilities/Math.h"

enum class TypeOfLight {
	Point = 0,
	Directional = 1,
	SpotLight = 2,
};

struct LightProperties {
	int id;
	Vector3 direction;
	Vector3 position;
	Vector3 colorAmb;
	Vector3 colorDif;
	Vector3 colorSpec;	
}; //Used as directional light properties as well

struct PointLightProperties : LightProperties
{
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

struct SpotLightProperties : LightProperties
{
	float cutOff = (float)cos(0.218f);
};

class Light{
public:
	Light() = default;
	Light(LightProperties& sop);
	void ChangeLightType(TypeOfLight type);
	TypeOfLight getType() const noexcept;	
	const Vector3& getPosition() const noexcept;
	const Vector3& getColorDif() const noexcept;
	const Vector3& getColorSpec() const noexcept;

private:
	LightProperties sop;
	TypeOfLight type;

};