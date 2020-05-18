#include "stdafx.h"
#include "ResourceManager.h"
#include "rapidxml.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <exception>

typedef rapidxml::xml_node<> xmlnode;
typedef rapidxml::xml_attribute<> xmlattr;

ResourceManager* ResourceManager::instance = NULL;

void ResourceManager::ReadXML()
{
	std::string xml = "../Resources/XML/resourceManager.xml";
	std::ifstream xmlf(xml);
	try {
		if (xmlf.bad() || xmlf.fail())
		{
			throw std::runtime_error("resourceManager.xml could not be opened");
		}
		else
		{
			std::cout << "resourceManager.xml was loaded" << std::endl;

			//Citire XML
			rapidxml::xml_document<> xmldoc;
			xmlnode* pRoot_node;

			//Parsare in buffer
			std::stringstream buffer;
			buffer << xmlf.rdbuf();
			xmlf.close();

			std::string content(buffer.str());
			xmldoc.parse<0>(&content[0]);

			//Acesare primului nod din XML (resourceManager)
			pRoot_node = xmldoc.first_node("resourceManager");
			//Pointer catre subnodul models
			xmlnode* pRoot_SubNode = pRoot_node->first_node("models");
			//Verifica daca subnodul exista
			if (!pRoot_SubNode)
			{
				std::cout << "Subnot 'models' doesn't exist in Resource Manager XML file";
			}
			//Variabila temporara pentru ID citit din XML
			int tempID = 0;
			//Structura temporare care vor fi mutat in map-ul respectiv			

			//Loop prin toate modelele
			for (xmlnode* pNode = pRoot_SubNode->first_node("model"); pNode; pNode = pNode->next_sibling())
			{
				//Structura temporara care va fi mutata in map
				std::unique_ptr<ModelResource> MR = std::make_unique<ModelResource>();
				//Citire ID din atributul model;
				tempID = std::stoi(pNode->first_attribute()->value());
				//Citire valoare subnod 'path' din 'model'
				MR->path = pNode->first_node("path")->value();
				//Mutare MR in ModelMap
				ModelMap[tempID] = std::move(MR);
			}

			//Cautare subnod Shaders in resourceManager
			pRoot_SubNode = pRoot_node->first_node("shaders");
			//Verificare subnod shader in XML
			if (!pRoot_SubNode)
			{
				std::cout << "Subnot 'shaders' doesn't exist in Resource Manager XML file";
			}
			//Loop prin toate shader-ele
			for (xmlnode* pNode = pRoot_SubNode->first_node("shader"); pNode; pNode = pNode->next_sibling())
			{
				//Structura temporara care va fi mutata in map
				std::unique_ptr<ShaderResource> SR = std::make_unique<ShaderResource>();
				//Citire ID din atributul shader			
				tempID = std::stoi(pNode->first_attribute()->value());
				//Citire valoare subnod 'vs' si 'vf' din 'shader'
				SR->vs = pNode->first_node("vs")->value();
				SR->fs = pNode->first_node("fs")->value();
				//Mutare SR in ShaderMap
				ShaderMap[tempID] = std::move(SR);
			}

			////Cautare subnod Textures in resourceManager
			pRoot_SubNode = pRoot_node->first_node("textures");
			//Verificare subnod Textures in XML
			if (!pRoot_SubNode)
			{
				std::cout << "Subnot 'textures' doesn't exist in Resource Manager XML file";
			}
			//Loop prin toate texturile
			for (xmlnode* pNode = pRoot_SubNode->first_node("texture"); pNode; pNode = pNode->next_sibling())
			{
				//Structura temporara care va fi mutata in map
				std::unique_ptr<TextureResource> TR = std::make_unique<TextureResource>();			
				//Citire ID din atributul texture			
				tempID = std::stoi(pNode->first_attribute("id")->value());
				//Citire type din atributul texture
				TR->type = pNode->first_attribute("id")->next_attribute("type")->value();
				//Citire min_filter din texture
				TR->min_filter = pNode->first_node("min_filter")->value();
				//Citire mag_filter din texture
				TR->mag_filter = pNode->first_node("mag_filter")->value();
				//CItire wrap_s din texture
				TR->wrap_s= pNode->first_node("wrap_s")->value();
				//CItire wrap_y din texture
				TR->wrap_t = pNode->first_node("wrap_t")->value();
				//Citire path din texture
				TR->path = pNode->first_node("path")->value();
				//Mutare TS in TextureMap
				TextureMap[tempID] = std::move(TR);
			}
		}
	}
	catch(std::runtime_error& xmlExcept)
	{
		std::cout << "xmlFile exception" << xmlExcept.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "caught an expection linked to resouceManager.xml file"<<std::endl;
	}
}

void ResourceManager::Init()
{
	ReadXML();	
}

ResourceManager* ResourceManager::GetInstance()
{	
	if (!instance)
	{
		instance = new ResourceManager;
	}
	return instance;
}

std::shared_ptr<Model> ResourceManager::loadModel(const int id)
{
	if (!Models[id])
	{
		//Creare model nou
		std::shared_ptr<Model> mdl = std::make_shared<Model>(ModelMap[id]->path);		
		//Adaugare model la vectorul de modele
		Models[id] = mdl;
	}
	return Models[id];
}

std::shared_ptr<Texture> ResourceManager::loadTexture(const int id)
{
	if (!Textures[id])
	{
		//Creare textura noua
		std::shared_ptr<Texture> tex = std::make_shared<Texture>(TextureMap[id]);			
		//Adaugare textura la vectorul de texturi
		Textures[id] = tex;
	}
	return Textures[id];
}

std::shared_ptr<Shader> ResourceManager::loadShader(const int id)
{
	if (!Shaders[id])
	{
		std::shared_ptr<Shader> sr = std::make_shared<Shader>(ShaderMap[id]);		
		sr->Load();
		Shaders[id] = sr;
	}
	return Shaders[id];
}

ResourceManager::ResourceManager()
{	
	glStringToInt["LINEAR"] = GL_LINEAR;
	glStringToInt["CLAMP_TO_EDGE"] = GL_CLAMP_TO_EDGE;
	glStringToInt["REPEAT"] = GL_REPEAT;
	glStringToInt["2d"] = GL_TEXTURE_2D;
	glStringToInt["cube"] = GL_TEXTURE_CUBE_MAP;
}

ResourceManager::~ResourceManager()
{	
	delete instance;
	instance = nullptr;	
}