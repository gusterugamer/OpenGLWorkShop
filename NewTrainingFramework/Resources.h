#pragma once
#include <string>
#include "../Utilities/glm/glm.hpp"
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
struct CamerasResource
{
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	float translationSpeed;
	float rotationSpeed;
	float fov;
	float nearZ;
	float farZ;
};
struct DebugSettings
{
	glm::vec3 objectOXcolor;
	glm::vec3 objectOYcolor;
	glm::vec3 objectOZcolor;
	glm::vec3 camOXcolor;
	glm::vec3 camOYcolor;
	glm::vec3 camOZcolor;
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
	glm::vec3 ambient = glm::vec3(1.0f,1.0f,1.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	TypeOfLight type = TypeOfLight::None;

	float range = 5.0f;
	float intensity = 1.0f;

	float cutOff = 0.97629f;
	float outterCutOff = 0.95371f;
};