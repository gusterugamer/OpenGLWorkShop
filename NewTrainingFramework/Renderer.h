#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "Model.h"
#include "SceneManager.h"
#include "SceneObject.h"

class Renderer {
	friend class SceneManager;
public:		
	static void Draw(const SceneObjectProperties& sop, const Model& model, Shader& shader, const Texture& tex);
	static void DrawBlendedTextures(const glm::mat4& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex);
	static void DrawBlendedTextures(const SceneObjectProperties& sop, const VertexBuffer& vb,const IndexBuffer& ib,Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex);
	static void DrawMultiTexture(const glm::mat4& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex);
	static void DrawSkyBox(const glm::mat4& modelMatrix, Model& model, Shader& shader, const Texture& tex);
	static void DrawDebug(const SceneObjectProperties& sop, const VertexBuffer& vb, const IndexBuffer& ib, Shader& shader, const Texture& tex);

private:	
	bool fog = false;	
	static SceneManager* SM;
};