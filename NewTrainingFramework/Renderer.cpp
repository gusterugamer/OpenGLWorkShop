#include "stdafx.h"
#include "Renderer.h"
#include "DebugModeFunctions.h"

SceneManager* Renderer::SM = nullptr;

void Renderer::Draw(const SceneObjectProperties& sop, const Model& model, Shader& shader, const Texture& tex)
{
	const Matrix& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const Matrix& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	//Buffers
	model.GetVd().Bind();
	model.GetIb().Bind();

	//Texture
	shader.AddTexture("u_texture", tex);	

	//ModelAttributes
	shader.AddAttrib("a_pos", sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	
	shader.AddAttrib("a_normal", sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, norm));
	
	shader.AddAttrib("a_uv", sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
	
	
	shader.AddUniform3f("u_cameraPos", SM->cameraMap[SM->activateCameraId]->position);
	shader.AddUniform3f("material.ambient", sop.mat.ambiental);
	shader.AddUniform3f("material.specular", sop.mat.specular);
	shader.AddUniform3f("material.diffuse", sop.mat.diffuse);
	shader.AddUniform1f("material.shininess", sop.mat.shininess);

	//Light
	for (unsigned int i = 0; i < SM->lights.size(); i++)
	{		
		std::shared_ptr<LightProperties> lp = SM->lights[i];
		switch (lp->type)
		{
		case TypeOfLight::Point:
		{			
			shader.AddUniformInArray3f("lights", i, "ambient", lp->ambient);			
			shader.AddUniformInArray3f("lights", i, "diffuse", lp->diffuse);			
			shader.AddUniformInArray3f("lights", i, "specular", lp->specular);
			shader.AddUniformInArray3f("lights", i, "position", lp->position);			
			shader.AddUniformInArray1i("lights", i, "type", (int)lp->type);
			shader.AddUniformInArray1f("lights", i, "range", lp->range);
			shader.AddUniformInArray1f("lights", i, "intensity", lp->intensity);				
		}
		break;
		case TypeOfLight::Directional:
		{
			shader.AddUniformInArray3f("lights", i, "ambient", lp->ambient);
			shader.AddUniformInArray3f("lights", i, "diffuse", lp->diffuse);
			shader.AddUniformInArray3f("lights", i, "specular", lp->specular);
			shader.AddUniformInArray3f("lights", i, "position", lp->position);
			shader.AddUniformInArray1i("lights", i, "type", (int)lp->type);			
			shader.AddUniformInArray1f("lights", i, "intensity", lp->intensity);
			shader.AddUniformInArray3f("lights", i, "direction", lp->direction);
		}
		break;
		case TypeOfLight::Spot:
		{
			shader.AddUniformInArray3f("lights", i, "ambient", lp->ambient);
			shader.AddUniformInArray3f("lights", i, "diffuse", lp->diffuse);
			shader.AddUniformInArray3f("lights", i, "specular", lp->specular);
			shader.AddUniformInArray3f("lights", i, "position", lp->position);
			shader.AddUniformInArray1i("lights", i, "type", (int)lp->type);
			shader.AddUniformInArray1f("lights", i, "range", lp->range);
			shader.AddUniformInArray1f("lights", i, "intensity", lp->intensity);
			shader.AddUniformInArray3f("lights", i, "direction", lp->direction);
			shader.AddUniformInArray1f("lights", i, "cutOff", lp->cutOff);
			shader.AddUniformInArray1f("lights", i, "outterCutOff", lp->outterCutOff);
		}
		break;
		}
	}

	//Matrices
	//Transform (or Model) matrix calculation
	Matrix RMx;
	Matrix RMy;
	Matrix RMz;
	Matrix SM;
	Matrix TM;
	Matrix modelMatrix;
	Matrix RM;

	RM = RMx.SetRotationX(sop.rotation.x) * RMy.SetRotationY(sop.rotation.y) *
		RMz.SetRotationZ(sop.rotation.z);
	SM = SM.SetScale((Vector3)sop.scale);
	TM = TM.SetTranslation((Vector3)sop.translation);	modelMatrix = SM * TM * RM;

	
	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);
	
	//DrawCall
	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));
}

void Renderer::DrawBlendedTextures(const Matrix& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const Matrix& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const Matrix& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	model.GetVd().Bind();
	model.GetIb().Bind();

	for (unsigned int i = 0; i < tex.size(); i++)
	{		
		shader.AddMultiTexture("u_texture", i, tex[i]);
	}

	shader.AddAttrib("a_pos", sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	shader.AddAttrib("a_uv", sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
	shader.AddAttrib("a_uvblend", sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uvblend));	

	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);

	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));
}

void Renderer::DrawBlendedTextures(const Matrix& modelMatrix, const VertexBuffer& vb, const IndexBuffer& ib, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const Matrix& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const Matrix& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	vb.Bind();
	ib.Bind();

	for (unsigned int i = 0; i < tex.size(); i++)
	{		
		shader.AddMultiTexture("u_texture", i, tex[i]);
	}

	shader.AddAttrib("a_pos", sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	shader.AddAttrib("a_uv", sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
	shader.AddAttrib("a_uvblend", sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uvblend));

	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));	
}

void Renderer::DrawMultiTexture(const Matrix& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const Matrix& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const Matrix& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	model.GetVd().Bind();
	model.GetIb().Bind();

	for (unsigned int i = 0; i < tex.size(); i++)
	{		
		shader.AddMultiTexture("u_texture", i, tex[i]);
	}

	shader.AddAttrib("a_pos", sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	shader.AddAttrib("a_uv", sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));

	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);

	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));
}

void Renderer::DrawSkyBox(const Matrix& modelMatrix, Model& model, Shader& shader, const Texture& tex)
{
	const Matrix& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const Matrix& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	model.GetVd().Bind();
	model.GetIb().Bind();

	shader.AddTexture("u_texture_cube", tex);	

	shader.AddAttrib("a_pos", sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));	

	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);

	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));	
}

void Renderer::DrawDebug(const SceneObjectProperties& sop, const VertexBuffer& vb, const IndexBuffer& ib, Shader& shader, const Texture& tex)
{
	const Matrix& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const Matrix& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	//Buffers
	vb.Bind();
	ib.Bind();

	//Texture
	shader.AddTexture("u_texture", tex);	

	//ModelAttributes
	shader.AddAttrib("a_pos", sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	shader.AddAttrib("a_uv", sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
	shader.AddAttrib("a_color", sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

	Matrix SM;
	Matrix TM;
	Matrix modelMatrix;

	SM = SM.SetScale((Vector3)sop.scale);
	TM = TM.SetTranslation((Vector3)sop.translation);

	modelMatrix = SM * TM;

	modelMatrix.m[0][0] = viewMatrix.m[0][0];
	modelMatrix.m[0][1] = viewMatrix.m[1][0];
	modelMatrix.m[0][2] = viewMatrix.m[2][0];
	modelMatrix.m[1][0] = viewMatrix.m[0][1];
	modelMatrix.m[1][1] = viewMatrix.m[1][1];
	modelMatrix.m[1][2] = viewMatrix.m[2][1];
	modelMatrix.m[2][0] = viewMatrix.m[0][2];
	modelMatrix.m[2][1] = viewMatrix.m[1][2];
	modelMatrix.m[2][2] = viewMatrix.m[2][2];

	Matrix LightMatrix = modelMatrix * (Matrix)(viewMatrix);
	
	shader.AddMatrix4fv("u_LightMatrix", 1, GL_FALSE, LightMatrix);
	shader.AddMatrix4fv("u_PerspectiveMatrix", 1, GL_FALSE, perspectiveMatrix);
	
	//DrawCall
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));
}