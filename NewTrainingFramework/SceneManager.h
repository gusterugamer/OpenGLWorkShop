#pragma once
#include "../Utilities/utilities.h"
#include "Resources.h"
#include "Camera.h"
#include "SceneObject.h" 
#include "DebugModeFunctions.h"

class SceneManager {
	friend class Renderer;	
private:

	//Resurse in care se citesc valorile din XML
	std::unordered_map<int, std::shared_ptr<Camera>> cameraMap;	
	std::unordered_map<int, std::shared_ptr<SceneObject>> sceneObjectsMap;		
	std::vector<std::shared_ptr<LightProperties>> lights;
	
	DebugSettings debugResource;
	int activateCameraId = 0;

	static SceneManager* instance;	

	void ReadXML();	
	SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator= (SceneManager&) = delete;

	///////////////////TEMPORAR////////////////////////	
	////////////////////TEMPORAR///////////////////////

public:	
	static SceneManager* GetInstance();
	
	void Init();
	void Draw();
	void Update(ESContext* esContext,const float& deltaTime);
	std::shared_ptr<Camera> GetCurrentCamera();		
	~SceneManager(); 	
};