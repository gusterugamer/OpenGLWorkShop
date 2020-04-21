#pragma once
#include "stdafx.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "SceneObject.h"

class Renderer {
public:	
	std::vector<Vertex> BatchedModels;	
	static void Draw(const VertexBuffer& vb,const IndexBuffer& ib,Shader& shader, const Texture& tex);
	static void DrawTerrain(const VertexBuffer& vb, const IndexBuffer& ib, Shader& shader, const std::vector<std::shared_ptr<Texture>>& texMap);
	static void DrawSkyBox(const VertexBuffer & vb, const IndexBuffer & ib, Shader & shader, const Texture& tex);

};