#include "stdafx.h"
#include "Light.h"
#include "OpenGLErrorChecking.h"

float Light::ambientLight = 0.5f;

Light::Light(LightProperties sop)
	:
	sop(sop)	
{
}

void Light::ChangeLightType(TypeOfLight type)
{
	sop.type = type;
}

TypeOfLight Light::getType() const noexcept
{
	return sop.type;
}

const Vector3& Light::getPosition() const noexcept
{
	return sop.translation;
}

const Vector3& Light::getColorDif() const noexcept
{
	return sop.colorDif;
}

const Vector3& Light::getColorSpec() const noexcept
{
	return sop.colorSpec;
}
