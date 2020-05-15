#pragma once
#include <string>
#include "../Utilities/Math.h"
#include <vector>
#include <unordered_map>

//Resurse pentru ResourceManager (XML)
struct ModelResource
{
	std::string path = "";
};
struct ShaderResource
{
	std::string vs = "";
	std::string fs = "";
};
struct TextureResource
{
	std::string type = "";
	std::string path = "";
	std::string min_filter = "";
	std::string mag_filter = "";
	std::string wrap_s = "";
	std::string wrap_t = "";	
	std::string normalMap = "";
};

//Resurse pentru SceneManager (XML)
struct BackgroundColorResource
{
	Vector3 RGB;
};
struct CamerasResource
{
	Vector3 position;
	Vector3 target;
	Vector3 up;
	GLfloat translationSpeed;
	GLfloat rotationSpeed;
	GLfloat fov;
	GLfloat nearZ;
	GLfloat farZ;
};
struct AmbientalLightResource
{
	Vector3 color;
	float ratio;
};
struct LightResource
{
	int associatedObject;
	Vector3 difuseColor;
	Vector3 specularColor;
	std::string type;
};
struct DebugSettings
{
	Vector3 objectOXcolor;
	Vector3 objectOYcolor;
	Vector3 objectOZcolor;
	Vector3 camOXcolor;
	Vector3 camOYcolor;
	Vector3 camOZcolor;
};

//Defineste tipul de lumini
enum class TypeOfLight {
	None = -1,
	Point = 0,
	Directional = 1,
	Spot = 2,
};

//structura generala ce contine proprietatile comune ale luminilor
struct LightProperties {
	Vector3 ambient;
	Vector3 direction;
	Vector3 position;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 color;
	TypeOfLight type;

	float range = 5.0f;
	float intensity = 1.0f;

	float cutOff;
	float outterCutOff;
};