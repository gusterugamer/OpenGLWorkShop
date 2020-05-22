#pragma once
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

struct Material {
	glm::vec3 ambiental = { 1.0f,1.0f,1.0f };
	glm::vec3 diffuse = { 1.0f,1.0f,1.0f };
	glm::vec3 specular = { 1.0f,1.0f,1.0f };
	float shininess = 0.5f;
};

struct SceneObjectProperties {
	//proprietatile unui obiect
	int modelId = -1;
	int shaderId = -1;
	std::string type;
	std::string blend;
	std::string name;
	int textureId = -1;
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
	std::vector<int> followCameraId;
	std::string trajectoryType;
	std::string trajectoryIteration;
	std::string trajectoryDirection;
	glm::vec3 trajectoryEndPoint;
	float trajectorySpeed = 0.0f;
	std::vector<int> lights;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f }; //culoare implicit daca modelul nu are textura incarcata(rosu)
	float reflectivity = 0.0f; //using before adding material class
	float shineDamper = 0.0f; //using before adding material class
	Material mat;
};

class SceneObject
{

protected:	
	//Pointeri catre resurele incarcate
	std::shared_ptr<Model> pMdl = nullptr;
	std::shared_ptr<Shader> pShader = nullptr;
	std::shared_ptr<Texture> pTex = nullptr;

	//Proprietati Obiect		
	SceneObjectProperties sop;

	glm::mat4 RMx;
	glm::mat4 RMy;
	glm::mat4 RMz;
	glm::mat4 SM;
	glm::mat4 TM;
	glm::mat4 modelMatrix;
	glm::mat4 RM;

public:

	virtual ~SceneObject();
	SceneObject() = default;
	SceneObject(SceneObjectProperties& sop);
	
	virtual void Draw();		
	virtual void Update(ESContext* esContext, const float& deltaTime);
};