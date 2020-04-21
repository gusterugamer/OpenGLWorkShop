#pragma once
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

struct SceneObjectProperties {
	//proprietatile unui obiect

	int modelId;
	int shaderId;
	std::string type;
	std::string blend;
	std::string name;
	int textureId;
	Vector3 translation;
	Vector3 rotation;
	Vector3 scale;
	std::vector<int> followCameraId;
	std::string trajectoryType;
	std::string trajectoryIteration;
	std::string trajectoryDirection;
	Vector3 trajectoryEndPoint;
	float trajectorySpeed;
	std::vector<int> lights;
	Vector3 color = { 1.0f, 1.0f, 1.0f }; //culoare implicit daca modelul nu are textura incarcata(rosu)
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

	//Matrice Model
	Matrix RMx;
	Matrix RMy;
	Matrix RMz;
	Matrix SM;
	Matrix TM;
	Matrix modelMatrix;
	Matrix RM;
	
public:

	virtual ~SceneObject();
	SceneObject() = default;
	SceneObject(SceneObjectProperties& sop);
	
	virtual void Draw();		
	virtual void Update(ESContext* esContext, const float& deltaTime);
};