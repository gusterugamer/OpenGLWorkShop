#pragma once
#include "../Utilities/Math.h"

enum class TypeOfLight {
	Point = 0,
	Directional = 1,
	SpotLight = 2,
};
struct LightProperties {
	int id;
	Vector3 translation;	
	Vector3 colorDif;
	Vector3 colorSpec;
	TypeOfLight type;
	float intensity = 1.0;
};

class Light{
public:
	Light() = default;
	Light(LightProperties sop);
	void ChangeLightType(TypeOfLight type);
	TypeOfLight getType() const noexcept;	
	const Vector3& getPosition() const noexcept;
	const Vector3& getColorDif() const noexcept;
	const Vector3& getColorSpec() const noexcept;

private:
	LightProperties sop;
public:
	static float ambientLight;
};