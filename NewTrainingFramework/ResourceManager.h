#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "Resources.h"

class ResourceManager {	
	
private:
	
	std::unordered_map<int, std::shared_ptr<ModelResource>> ModelMap;
	std::unordered_map<int, std::shared_ptr<ShaderResource>> ShaderMap;
	std::unordered_map<int, std::shared_ptr<TextureResource>> TextureMap; 

	std::unordered_map<int, std::shared_ptr<Model>> Models;
	std::unordered_map<int, std::shared_ptr<Shader>> Shaders;
	std::unordered_map<int, std::shared_ptr<Texture>> Textures;

	void ReadXML();

	static ResourceManager* instance;

	ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator= (ResourceManager&) = delete;
public:
	//Map care ajuta la conversia stringului din XML in int pentru macros
	

	~ResourceManager();	
	void Init();
	static ResourceManager* GetInstance();
	std::unordered_map<std::string, GLint> glStringToInt;

	std::shared_ptr<Model> loadModel(const int id);	
	std::shared_ptr<Shader> loadShader(const int id);	
	std::shared_ptr<Texture> loadTexture(const int id);
};


