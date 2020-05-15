#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "SceneObject.h"

class Renderer {
	friend class SceneManager;
public:		
	static void Draw(const SceneObjectProperties& sop, const Model& model, Shader& shader, const Texture& tex);
	static void DrawBlendedTextures(const Matrix& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex);
	static void DrawBlendedTextures(const Matrix& modelMatrix, const VertexBuffer& vb,const IndexBuffer& ib,Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex);
	static void DrawMultiTexture(const Matrix& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex);
	static void DrawSkyBox(const Matrix& modelMatrix, Model& model, Shader& shader, const Texture& tex);	
	static void DrawDebug(const SceneObjectProperties& sop, const VertexBuffer& vb, const IndexBuffer& ib, Shader& shader, const Texture& tex);
private:	
	bool fog = false;
	static std::vector<std::shared_ptr<LightProperties>>* pLights;
};