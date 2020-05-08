#include "stdafx.h"
#include "Light.h"
#include "OpenGLErrorChecking.h"

Light::Light(LightProperties& sop)
	:
	sop(sop)
{
}

void Light::ChangeLightType(TypeOfLight type)
{
	switch (type)
	{
	case TypeOfLight::Point:
	{
		if (this->type != type)
		{
			PointLightProperties tempSop;
			tempSop.direction = sop.direction;
			tempSop.position = sop.position;
			tempSop.colorAmb = sop.colorAmb;
			tempSop.colorDif = sop.colorDif;
			tempSop.colorSpec = sop.colorSpec;
			sop = tempSop;
			//Todo: Change light symbol for when added
		}
	}
	break;

	case TypeOfLight::Directional:
	{
		if (this->type != type)
		{
			LightProperties tempSop;
			tempSop.direction = sop.direction;
			tempSop.position = sop.position;
			tempSop.colorAmb = sop.colorAmb;
			tempSop.colorDif = sop.colorDif;
			tempSop.colorSpec = sop.colorSpec;
			sop = tempSop;
			//Todo: Change light symbol for when added
		}
	}
	break;

	case TypeOfLight::SpotLight:
	{
		if (this->type != type)
		{
			SpotLightProperties tempSop;
			tempSop.direction = sop.direction;
			tempSop.position = sop.position;
			tempSop.colorAmb = sop.colorAmb;
			tempSop.colorDif = sop.colorDif;
			tempSop.colorSpec = sop.colorSpec;
			sop = tempSop;
			//Todo: Change light symbol for when added
		}
	}
	break;

	}
}

TypeOfLight Light::getType() const noexcept
{
	return type;
}

const Vector3& Light::getPosition() const noexcept
{
	return sop.position;
}

const Vector3& Light::getColorDif() const noexcept
{
	return sop.colorDif;
}

const Vector3& Light::getColorSpec() const noexcept
{
	return sop.colorSpec;
}
