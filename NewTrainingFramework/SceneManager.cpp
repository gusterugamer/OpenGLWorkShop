#include "stdafx.h"
#include "rapidxml.hpp"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Terrain.h"
#include "Fire.h"
#include "Skybox.h"
#include "Light.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

typedef rapidxml::xml_node<> xmlnode;
typedef rapidxml::xml_attribute<> xmlattr;

SceneManager* SceneManager::instance = NULL;

void SceneManager::ReadXML()
{	
	const char* xml = "../Resources/XML/sceneManager.xml";
	std::ifstream xmlf(xml, std::ifstream::binary);
	try {
		if (xmlf.bad() || xmlf.fail())
		{
			throw std::runtime_error("sceneManager.xml could not be opened");
		}
		else		
		{
			std::cout << "sceneXML was loaded" << std::endl;

			//Citire XML
			rapidxml::xml_document<> xmldoc;
			xmlnode* pRoot_node = nullptr;		
			//std::string doc = buffer.str();

			//Parsare in buffer			
			
			std::stringstream buffer;		
			
			buffer << xmlf.rdbuf();		
			std::string doc= buffer.str();			
			
			xmldoc.parse<0>(&doc[0]);
			xmlf.close();			
			
			auto start = std::chrono::high_resolution_clock::now();
			//Variabila temporara pentru ID citit din XML
			int tempID = 0;
			//Acesare primului nod din XML (sceneManager)
			pRoot_node = xmldoc.first_node("sceneManager");		 
			
			//Pointer catre subnodul cameras
			 xmlnode* pRoot_SubNode = pRoot_node->first_node("cameras");
			//Verifica daca subnodul backgroundColor exista
			if (!pRoot_SubNode)
			{
				std::cout << "Subnod 'cameras' doesn't exist in Scene Manager XML file";
			}
			else
			{
				for (xmlnode* pNode = pRoot_SubNode->first_node("camera"); pNode; pNode = pNode->next_sibling())
				{
					//ObjectsResource temporar care va fi mutat in map
					std::unique_ptr<CamerasResource> CR = std::make_unique<CamerasResource>();
					//Variabila temporara pentru ID-ul camerei pentru adaugarea in map
					tempID = std::stoi(pNode->first_attribute("id")->value());
					//Citirea vectorului pozitie camera din xml si pasarea in resursa temporara
					CR->position.x = std::stof(pNode->first_node("position")->first_node("x")->value());
					CR->position.y = std::stof(pNode->first_node("position")->first_node("y")->value());
					CR->position.z = std::stof(pNode->first_node("position")->first_node("z")->value());
					//Citirea vectorului target din xml si pasarea in resursa temporara			
					CR->target.x = std::stof(pNode->first_node("target")->first_node("x")->value());
					CR->target.y = std::stof(pNode->first_node("target")->first_node("y")->value());
					CR->target.z = std::stof(pNode->first_node("target")->first_node("z")->value());
					//Citirea vectorului up din xml si pasarea in resursa temporara
					CR->up.x = std::stof(pNode->first_node("up")->first_node("x")->value());
					CR->up.y = std::stof(pNode->first_node("up")->first_node("y")->value());
					CR->up.z = std::stof(pNode->first_node("up")->first_node("z")->value());
					//Citirea vectorului translationSpeed din xml si pasarea in resursa temporara
					CR->translationSpeed = std::stof(pNode->first_node("translationSpeed")->value());
					// Citirea vectorului rotationSpeed din xml si pasarea in resursa temporara
					CR->rotationSpeed = std::stof(pNode->first_node("rotationSpeed")->value());
					// Citirea vectorului fov din xml si pasarea in resursa temporara
					CR->fov = std::stof(pNode->first_node("fov")->value());
					// Citirea vectorului far din xml si pasarea in resursa temporara
					CR->nearZ = std::stof(pNode->first_node("near")->value());
					// Citirea vectorului farZ din xml si pasarea in resursa temporara
					CR->farZ = std::stof(pNode->first_node("far")->value());
					//Mutare resursa temporara in vectorul de pointeri de resurse
					//Constructie camera cu resursele cittite si mutarea in vectorul cameras
					std::unique_ptr<Camera> cam = std::make_unique<Camera>(CR->position, CR->target, CR->fov, CR->nearZ, CR->farZ, CR->translationSpeed, CR->rotationSpeed);

					cameraMap[tempID] = std::move(cam);
				}
			}
			//Pointer catre subnodul objects
			pRoot_SubNode = pRoot_node->first_node("objects");
			//Verificare daca subnodul
			if (!pRoot_SubNode)
			{
				std::cout << "Subnod 'objects' doesn't exist in Scene Manager XML file";
			}
			else
			{
				for (xmlnode* pNode = pRoot_SubNode->first_node("object"); pNode; pNode = pNode->next_sibling())
				{
					if (std::string(pNode->first_node("type")->value()) == "terrain")
					{					
						//Creare model temporar
						TerrainProperties tp;
						//Citire ID obiect pentru contruirea vectorului					
						tempID = std::stoi(pNode->first_attribute("id")->value());
						//Citire proprietati												
						tp.offSetY = std::stof(pNode->first_node("offsetY")->value());
						tp.dimensiuneCelule = std::stoi(pNode->first_node("dimcelula")->value());
						tp.numarCelule = std::stoi(pNode->first_node("celule")->value());						
						tp.modelId = 0;
						tp.name = pNode->first_node("name")->value();
						tp.blend = pNode->first_node("blend")->value();
						tp.shaderId = std::stoi(pNode->first_node("shader")->value());
						for (xmlnode* ppNode = pNode->first_node("textures")->first_node("texture"); ppNode; ppNode = ppNode->next_sibling())
						{
							tp.textureId.push_back(std::stoi(ppNode->first_attribute("id")->value()));
						}

						tp.translation.x = std::stof(pNode->first_node("position")->first_node("x")->value());
						tp.translation.y = std::stof(pNode->first_node("position")->first_node("y")->value());
						tp.translation.z = std::stof(pNode->first_node("position")->first_node("z")->value());
						
						tp.rotation.x = std::stof(pNode->first_node("rotation")->first_node("x")->value());
						tp.rotation.y = std::stof(pNode->first_node("rotation")->first_node("y")->value());
						tp.rotation.z = std::stof(pNode->first_node("rotation")->first_node("z")->value());
						
						tp.scale.x = std::stof(pNode->first_node("scale")->first_node("x")->value());
						tp.scale.y = std::stof(pNode->first_node("scale")->first_node("y")->value());
						tp.scale.z = std::stof(pNode->first_node("scale")->first_node("z")->value());

						tp.height.x = std::stof(pNode->first_node("inaltimi")->first_node("r")->value());
						tp.height.y = std::stof(pNode->first_node("inaltimi")->first_node("g")->value());
						tp.height.z = std::stof(pNode->first_node("inaltimi")->first_node("b")->value());

						for (xmlnode* ppNode = pNode->first_node("lights")->first_node("light"); ppNode; ppNode = ppNode->next_sibling())
						{
							tp.lights.push_back(std::stoi(ppNode->value()));
						}
						sceneObjectsMap[tempID] = std::make_shared<Terrain>(tp);
					}
					else if (std::string(pNode->first_node("type")->value()) == "normal")
					{
						//ObjectsResource temporar care va fi mutat in vector
						SceneObjectProperties OR;
						//Citire ID obiect pentru contruirea vectorului					
						tempID = std::stoi(pNode->first_attribute("id")->value());
						//Citire model ID din XML si pasarea in resursa temporara					
						OR.modelId = std::stoi(pNode->first_node("model")->value());
						//Citire shaderID din XML si pasarea in resursa temporara
						OR.shaderId = std::stoi(pNode->first_node("shader")->value());
						//Citire blend din XML si pasarea in resursa temporara
						OR.blend = pNode->first_node("blend")->value();
						//Citire name din XML si pasarea in resursa temporara
						OR.name = pNode->first_node("name")->value();
						//Citire textures din XML si pasarea in resursa temporara					
						OR.textureId = std::stoi(pNode->first_node("textures")->first_node("texture")->first_attribute("id")->value());						
						//Citire type din XML si pasarea in resursa temporara
						OR.type = pNode->first_node("type")->value();
						//Citire pozitia la care trebuie translatat obiectul din XML si pasarea in resursa temporara
						OR.translation.x = std::stof(pNode->first_node("position")->first_node("x")->value());
						OR.translation.y = std::stof(pNode->first_node("position")->first_node("y")->value());
						OR.translation.z = std::stof(pNode->first_node("position")->first_node("z")->value());
						//Citire vectorul la care trebuie rotit obiectul din XML si pasarea in resursa temporara
						OR.rotation.x = std::stof(pNode->first_node("rotation")->first_node("x")->value());
						OR.rotation.y = std::stof(pNode->first_node("rotation")->first_node("y")->value());
						OR.rotation.z = std::stof(pNode->first_node("rotation")->first_node("z")->value());
						//Citire vectorul la care trebuie scalat obiectul din XML si pasarea in resursa temporara
						OR.scale.x = std::stof(pNode->first_node("scale")->first_node("x")->value());
						OR.scale.y = std::stof(pNode->first_node("scale")->first_node("y")->value());
						OR.scale.z = std::stof(pNode->first_node("scale")->first_node("z")->value());
						//Citire camere care trebuie sa se uite la obiectul din XML si pasarea in resursa temporara
						if (xmlnode* ppNode = pNode->first_node("followingCamera"))
						{
							for (xmlnode* ppNode = pNode->first_node("followingCamera")->first_node("cameras")->first_node("camera"); ppNode; ppNode = ppNode->next_sibling())
							{
								OR.followCameraId.push_back(std::stoi(ppNode->value()));
							}
						}
						//Citire atribut direction traiectorie pe care o are obiectul din XML si pasarea in resursa temporara
						if (pNode->first_node("trajectory"))
						{
							OR.trajectoryDirection = pNode->first_node("trajectory")->first_attribute("direction")->value();
							//Citire atribut type traiectorie pe care o are obiectul din XML si pasarea in resursa temporara					
							OR.trajectoryType = pNode->first_node("trajectory")->first_attribute("type")->value();
							//Citire atribut iteration traiectorie pe care o are obiectul din XML si pasarea in resursa temporara
							OR.trajectoryIteration = pNode->first_node("trajectory")->first_attribute("iteration")->value();
							//Citire EndPoint din XML si pasarea in resursa temporara
							OR.trajectoryEndPoint.x = std::stof(pNode->first_node("trajectory")->first_node("endPoint")->first_node("x")->value());
							OR.trajectoryEndPoint.y = std::stof(pNode->first_node("trajectory")->first_node("endPoint")->first_node("y")->value());
							OR.trajectoryEndPoint.z = std::stof(pNode->first_node("trajectory")->first_node("endPoint")->first_node("z")->value());
							//Citire speed din XML si pasarea in resursa temporara
							OR.trajectorySpeed = std::stof(pNode->first_node("trajectory")->first_node("speed")->value());
						}
						//Citire lights din XML si pasarea in resursa temporara 										
						for (xmlnode* ppNode = pNode->first_node("lights")->first_node("light"); ppNode; ppNode = ppNode->next_sibling())
						{
							OR.lights.push_back(std::stoi(ppNode->value()));							
						}							
						sceneObjectsMap[tempID] = std::make_shared<SceneObject>(OR);
					}	
					else if (std::string(pNode->first_node("type")->value()) == "fire")
					{
						//Id pentru punere in vectorul de obiecte
						tempID = std::stoi(pNode->first_attribute("id")->value());
						//Creare model temporar
						FireProperties fp;
						//Citire proprietati						
						fp.modelId = tempID;
						fp.name = pNode->first_node("name")->value();
						fp.blend = pNode->first_node("blend")->value();
						fp.shaderId = std::stoi(pNode->first_node("shader")->value());
						for (xmlnode* ppNode = pNode->first_node("textures")->first_node("texture"); ppNode; ppNode = ppNode->next_sibling())
						{
							fp.textureId.push_back(std::stoi(ppNode->first_attribute("id")->value()));
						}

						fp.translation.x = std::stof(pNode->first_node("position")->first_node("x")->value());
						fp.translation.y = std::stof(pNode->first_node("position")->first_node("y")->value());
						fp.translation.z = std::stof(pNode->first_node("position")->first_node("z")->value());
						
						fp.rotation.x = std::stof(pNode->first_node("rotation")->first_node("x")->value());
						fp.rotation.y = std::stof(pNode->first_node("rotation")->first_node("y")->value());
						fp.rotation.z = std::stof(pNode->first_node("rotation")->first_node("z")->value());
						
						fp.scale.x = std::stof(pNode->first_node("scale")->first_node("x")->value());
						fp.scale.y = std::stof(pNode->first_node("scale")->first_node("y")->value());
						fp.scale.z = std::stof(pNode->first_node("scale")->first_node("z")->value());

						for (xmlnode* ppNode = pNode->first_node("lights")->first_node("light"); ppNode; ppNode = ppNode->next_sibling())
						{
							fp.lights.push_back(std::stoi(ppNode->value()));
						}
						fp.dispMax = std::stof(pNode->first_node("dispMax")->value());
						sceneObjectsMap[tempID] = std::make_shared<Fire>(fp); 
					}
					else if (std::string(pNode->first_node("type")->value()) == "skybox")
					{
					//Id pentru punere in vectorul de obiecte
					tempID = std::stoi(pNode->first_attribute("id")->value());
					//Creare model temporar
					SkyBoxProps sp;
					//Citire proprietati						
					sp.modelId = tempID;
					sp.name = pNode->first_node("name")->value();
					sp.blend = pNode->first_node("blend")->value();
					sp.shaderId = std::stoi(pNode->first_node("shader")->value());
					sp.textureId = std::stoi(pNode->first_node("textures")->first_node("texture")->first_attribute("id")->value());

					sp.translation.x = std::stof(pNode->first_node("position")->first_node("x")->value());
					sp.translation.y = std::stof(pNode->first_node("position")->first_node("y")->value());
					sp.translation.z = std::stof(pNode->first_node("position")->first_node("z")->value());
					
					sp.rotation.x = std::stof(pNode->first_node("rotation")->first_node("x")->value());
					sp.rotation.y = std::stof(pNode->first_node("rotation")->first_node("y")->value());
					sp.rotation.z = std::stof(pNode->first_node("rotation")->first_node("z")->value());
					
					sp.scale.x = std::stof(pNode->first_node("scale")->first_node("x")->value());
					sp.scale.y = std::stof(pNode->first_node("scale")->first_node("y")->value());
					sp.scale.z = std::stof(pNode->first_node("scale")->first_node("z")->value());

					for (xmlnode* ppNode = pNode->first_node("lights")->first_node("light"); ppNode; ppNode = ppNode->next_sibling())
					{
						sp.lights.push_back(std::stoi(ppNode->value()));
					}					
					sceneObjectsMap[tempID] = std::make_shared<Skybox>(sp);
					}
				}
			}			
			//Verificare daca exista subnodul ambientalLight

			//Pointer catre subnodul lights
			pRoot_SubNode = pRoot_node->first_node("lights");
			//Verificare daca exista subnodul Light
			if (!pRoot_SubNode)
			{
				std::cout << "Subnod 'lights' doesn't exist in Scene Manager XML file";
			}
			else
			{
				glm::vec3 ambiental;
				int shaderId = 7;
				if (xmlnode* pSubNode = pRoot_SubNode->first_node("ambiental"))
				{					
					ambiental.x = std::stof(pSubNode->first_node("color")->first_node("r")->value());
					ambiental.y = std::stof(pSubNode->first_node("color")->first_node("b")->value());
					ambiental.z = std::stof(pSubNode->first_node("color")->first_node("g")->value());
				}				
				if (xmlnode* pSubNode = pRoot_SubNode->first_node("shaderId"))
				{
					shaderId = std::stoi(pSubNode->value());
				}
				for (xmlnode* ppNode = pRoot_SubNode->first_node("light"); ppNode; ppNode = ppNode->next_sibling())
				{
					//Variabila temporara ID pentru construirea vectorului lights
					tempID = std::stoi(ppNode->first_attribute("id")->value());
					std::string type = ppNode->first_attribute("type")->value();
					//Resursa temporara care va fi mutata in vectorul de pointeri lightMap;

					if (type == "point")
					{
						std::shared_ptr<LightProperties> LR = std::make_shared<LightProperties>();
						std::unique_ptr<SceneObjectProperties> sop = std::make_unique<SceneObjectProperties>();

						//Lumina ambianta
						LR->ambient.x = ambiental.x;
						LR->ambient.y = ambiental.y;
						LR->ambient.z = ambiental.z;
						//Citire difuseColor din XML si pasare catre resursa temporara
						LR->diffuse.x = std::stof(ppNode->first_node("diffuseColor")->first_node("r")->value());
						LR->diffuse.y = std::stof(ppNode->first_node("diffuseColor")->first_node("g")->value());
						LR->diffuse.z = std::stof(ppNode->first_node("diffuseColor")->first_node("b")->value());
						//Citire specularColor din XML si pasare catre resursa temporara
						LR->specular.x = std::stof(ppNode->first_node("specularColor")->first_node("r")->value());
						LR->specular.y = std::stof(ppNode->first_node("specularColor")->first_node("g")->value());
						LR->specular.z = std::stof(ppNode->first_node("specularColor")->first_node("b")->value());
						LR->range = std::stof(ppNode->first_node("range")->value());						
						LR->intensity = std::stof(ppNode->first_node("intensity")->value());						
						
						//Citire type din XML si pasare catre resursa temporara
						LR->type = TypeOfLight::Point;					

						int id = std::stoi(ppNode->first_node("id")->value());
						sop->name = ppNode->first_node("name")->value();
						sop->shaderId = shaderId;						
						sop->textureId = std::stoi(ppNode->first_node("textureId")->value());
						sop->translation.x = std::stof(ppNode->first_node("translation")->first_node("x")->value());
						sop->translation.y = std::stof(ppNode->first_node("translation")->first_node("y")->value());
						sop->translation.z = std::stof(ppNode->first_node("translation")->first_node("z")->value());
						LR->position = sop->translation;
						sop->scale = glm::vec3(1.0, 1.0, 1.0);
						sceneObjectsMap[id] = std::make_shared<Light>(*sop.release(), *LR);		
						lights.push_back(LR);
					}					

					else if (type == "spot")
					{
						std::shared_ptr<LightProperties> LR = std::make_shared<LightProperties>();
						std::unique_ptr<SceneObjectProperties> sop = std::make_unique<SceneObjectProperties>();

						//Lumina ambianta
						LR->ambient.x = ambiental.x;
						LR->ambient.y = ambiental.y;
						LR->ambient.z = ambiental.z;
						//Citire difuseColor din XML si pasare catre resursa temporara
						LR->diffuse.x = std::stof(ppNode->first_node("diffuseColor")->first_node("r")->value());
						LR->diffuse.y = std::stof(ppNode->first_node("diffuseColor")->first_node("g")->value());
						LR->diffuse.z = std::stof(ppNode->first_node("diffuseColor")->first_node("b")->value());
						//Citire specularColor din XML si pasare catre resursa temporara
						LR->specular.x = std::stof(ppNode->first_node("specularColor")->first_node("r")->value());
						LR->specular.y = std::stof(ppNode->first_node("specularColor")->first_node("g")->value());
						LR->specular.z = std::stof(ppNode->first_node("specularColor")->first_node("b")->value());
						LR->cutOff = std::stof(ppNode->first_node("cutOff")->value());
						LR->outterCutOff = std::stof(ppNode->first_node("outterCutOff")->value());
						LR->direction.x = std::stof(ppNode->first_node("direction")->first_node("x")->value());
						LR->direction.y = std::stof(ppNode->first_node("direction")->first_node("y")->value());
						LR->direction.z = std::stof(ppNode->first_node("direction")->first_node("z")->value());
						LR->intensity = std::stof(ppNode->first_node("intensity")->value());
						LR->range = std::stof(ppNode->first_node("range")->value());

						//Citire type din XML si pasare catre resursa temporara
						LR->type = TypeOfLight::Spot;

						int id = std::stoi(ppNode->first_node("id")->value());
						sop->name = ppNode->first_node("name")->value();
						sop->shaderId = shaderId;
						sop->textureId = std::stoi(ppNode->first_node("textureId")->value());
						sop->translation.x = std::stof(ppNode->first_node("translation")->first_node("x")->value());
						sop->translation.y = std::stof(ppNode->first_node("translation")->first_node("y")->value());
						sop->translation.z = std::stof(ppNode->first_node("translation")->first_node("z")->value());
						LR->position = sop->translation;
						sop->scale = glm::vec3(1.0, 1.0, 1.0);
						sceneObjectsMap[id] = std::make_shared<Light>(*sop.release(), *LR);
						lights.push_back(LR);
					}
					else if (type == "directional")
					{
						std::shared_ptr<LightProperties> LR = std::make_shared<LightProperties>();
						std::unique_ptr<SceneObjectProperties> sop = std::make_unique<SceneObjectProperties>();

						//Lumina ambianta
						LR->ambient.x = ambiental.x;
						LR->ambient.y = ambiental.y;
						LR->ambient.z = ambiental.z;
						//Citire difuseColor din XML si pasare catre resursa temporara
						LR->diffuse.x = std::stof(ppNode->first_node("diffuseColor")->first_node("r")->value());
						LR->diffuse.y = std::stof(ppNode->first_node("diffuseColor")->first_node("g")->value());
						LR->diffuse.z = std::stof(ppNode->first_node("diffuseColor")->first_node("b")->value());
						//Citire specularColor din XML si pasare catre resursa temporara
						LR->specular.x = std::stof(ppNode->first_node("specularColor")->first_node("r")->value());
						LR->specular.y = std::stof(ppNode->first_node("specularColor")->first_node("g")->value());
						LR->specular.z = std::stof(ppNode->first_node("specularColor")->first_node("b")->value());

						LR->direction.x = std::stof(ppNode->first_node("direction")->first_node("x")->value());
						LR->direction.y = std::stof(ppNode->first_node("direction")->first_node("y")->value());
						LR->direction.z = std::stof(ppNode->first_node("direction")->first_node("z")->value());
						LR->intensity = std::stof(ppNode->first_node("intensity")->value());

						//Citire type din XML si pasare catre resursa temporara
						LR->type = TypeOfLight::Directional;

						int id = std::stoi(ppNode->first_node("id")->value());
						sop->name = ppNode->first_node("name")->value();
						sop->shaderId = shaderId;
						sop->textureId = std::stoi(ppNode->first_node("textureId")->value());
						sop->translation.x = std::stof(ppNode->first_node("translation")->first_node("x")->value());
						sop->translation.y = std::stof(ppNode->first_node("translation")->first_node("y")->value());
						sop->translation.z = std::stof(ppNode->first_node("translation")->first_node("z")->value());						
						sop->scale = glm::vec3(1.0, 1.0, 1.0);
						sceneObjectsMap[id] = std::make_shared<Light>(*sop.release(), *LR);
						lights.push_back(LR);
					}
				}
			}
			//Citire active camera din XML
			activateCameraId = std::stoi(pRoot_node->first_node("activeCamera")->value());
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = end - start;
			float f = duration.count();
		}
	}
	catch (const std::runtime_error& sceXml)
	{
		std::cout << sceXml.what();
	}
	catch (...)
	{
		std::cout << "Caught an expection linked to resouceManager.xml file" << std::endl;
	}	
}

SceneManager* SceneManager::GetInstance()
{
	if (!instance)
	{
		instance = new SceneManager;
	}
	return instance;
}

void SceneManager::Init()
{	
	//Citire date din sceneManager.xml
	ReadXML();	
	Renderer::SM = this;		
}

void SceneManager::Draw()
{	
	//Desenarea tuturor obiectelor care au fost deja incarcate
	for (auto n : sceneObjectsMap)
	{
		n.second->Draw();		
	}		
}

void SceneManager::Update(ESContext* esContext,const float& deltaTime)
{	
	for (auto n : sceneObjectsMap)
	{
		n.second->Update(esContext,deltaTime);
	}	
	cameraMap[activateCameraId]->Update(esContext, deltaTime);	
}

std::shared_ptr<Camera> SceneManager::GetCurrentCamera()
{
	return cameraMap[activateCameraId];
}

SceneManager::~SceneManager()
{
	delete instance;
	instance = nullptr;
}
