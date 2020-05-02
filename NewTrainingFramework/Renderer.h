#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "SceneObject.h"

class Renderer {
public:		
	static void Draw(const SceneObjectProperties& sop, const Model& model, Shader& shader, const Texture& tex, Light& light);
	static void DrawBlendedTextures(const Matrix& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex);
	static void DrawBlendedTextures(const Matrix& modelMatrix, const VertexBuffer& vb,const IndexBuffer& ib,Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex);
	static void DrawMultiTexture(const Matrix& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex);
	static void DrawSkyBox(const Matrix& modelMatrix, Model& model, Shader& shader, const Texture& tex);	
private:
	float ambientalLight = 0.8f;
	bool fog = false;

};