#include "stdafx.h"
#include "Renderer.h"
#include "DebugModeFunctions.h"
#include "../Utilities/glm/gtc/quaternion.hpp"
#include "../Utilities/glm/gtx/quaternion.hpp"

SceneManager* Renderer::SM = nullptr;

void Renderer::Draw(const SceneObjectProperties& sop, const Model& model, Shader& shader, const Texture& tex)
{
	const glm::mat4& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const glm::mat4& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	//Buffers
	model.GetVd().Bind();
	model.GetIb().Bind();

	//Texture
	shader.AddTexture("u_texture", tex);	

	//ModelAttributes
	shader.AddAttrib("a_pos", sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	
	shader.AddAttrib("a_normal", sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, norm));
	
	shader.AddAttrib("a_uv", sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
	
	
	shader.AddUniform3f("u_cameraPos", SM->cameraMap[SM->activateCameraId]->position);
	shader.AddUniform3f("material.ambient", sop.mat.ambiental);
	shader.AddUniform3f("material.specular", sop.mat.specular);
	shader.AddUniform3f("material.diffuse", sop.mat.diffuse);
	shader.AddUniform1f("material.shininess", sop.mat.shininess);
	shader.AddUniform3f("u_skyColor", glm::vec3(0.5f, 0.5f, 0.5f));

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
	//Transform (or Model) glm::mat4 calculation
	glm::mat4 RM;
	glm::mat4 SM;
	glm::mat4 TM;
	glm::mat4 modelMatrix;

	glm::quat quaternion = glm::quat(glm::radians(sop.rotation));
	RM = glm::toMat4(quaternion);
	SM = glm::scale(glm::mat4(1.0f), sop.scale);
	TM = glm::translate(glm::mat4(1.0f), sop.translation);
	modelMatrix =TM * RM * SM;

	
	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);
	
	//DrawCall
	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));
}

void Renderer::DrawBlendedTextures(const glm::mat4& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const glm::mat4& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const glm::mat4& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	model.GetVd().Bind();
	model.GetIb().Bind();

	for (unsigned int i = 0; i < tex.size(); i++)
	{		
		shader.AddMultiTexture("u_texture", i, tex[i]);
	}

	shader.AddAttrib("a_pos", sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	shader.AddAttrib("a_uv", sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
	shader.AddAttrib("a_uvblend", sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uvblend));	

	

	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);

	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));
}

void Renderer::DrawBlendedTextures(const SceneObjectProperties& sop, const VertexBuffer& vb, const IndexBuffer& ib, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const glm::mat4& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const glm::mat4& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	vb.Bind();
	ib.Bind();

	for (unsigned int i = 0; i < tex.size(); i++)
	{		
		shader.AddMultiTexture("u_texture", i, tex[i]);
	}

	shader.AddAttrib("a_pos", sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	shader.AddAttrib("a_uv", sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
	shader.AddAttrib("a_uvblend", sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uvblend));
	shader.AddAttrib("a_normal", sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, norm));

	shader.AddUniform3f("u_cameraPos", SM->cameraMap[SM->activateCameraId]->position);
	shader.AddUniform3f("material.ambient", sop.mat.ambiental);
	shader.AddUniform3f("material.specular", sop.mat.specular);
	shader.AddUniform3f("material.diffuse", sop.mat.diffuse);
	shader.AddUniform1f("material.shininess", sop.mat.shininess);
	shader.AddUniform3f("u_skyColor", glm::vec3(0.5f, 0.5f, 0.5f));

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
	//Transform (or Model) glm::mat4 calculation
	glm::mat4 RM;
	glm::mat4 SM;
	glm::mat4 TM;
	glm::mat4 modelMatrix;

	glm::quat quaternion = glm::quat(glm::radians(sop.rotation));
	RM = glm::toMat4(quaternion);
	SM = glm::scale(glm::mat4(1.0f), sop.scale);
	TM = glm::translate(glm::mat4(1.0f), sop.translation);
	modelMatrix = TM * RM * SM;

	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));	
}

void Renderer::DrawMultiTexture(const glm::mat4& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const glm::mat4& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const glm::mat4& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	model.GetVd().Bind();
	model.GetIb().Bind();

	for (unsigned int i = 0; i < tex.size(); i++)
	{		
		shader.AddMultiTexture("u_texture", i, tex[i]);
	}

	shader.AddAttrib("a_pos", sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	shader.AddAttrib("a_uv", sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));

	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);

	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));
}

void Renderer::DrawSkyBox(const glm::mat4& modelMatrix, Model& model, Shader& shader, const Texture& tex)
{
	const glm::mat4& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const glm::mat4& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	model.GetVd().Bind();
	model.GetIb().Bind();

	shader.AddTexture("u_texture_cube", tex);	

	shader.AddAttrib("a_pos", sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));	

	shader.AddMatrix4fv("u_view", 1, GL_FALSE, viewMatrix);
	shader.AddMatrix4fv("u_model", 1, GL_FALSE, modelMatrix);
	shader.AddMatrix4fv("u_perspective", 1, GL_FALSE, perspectiveMatrix);

	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));	
}

void Renderer::DrawDebug(const SceneObjectProperties& sop, const VertexBuffer& vb, const IndexBuffer& ib, Shader& shader, const Texture& tex)
{

	const glm::mat4& viewMatrix = SM->cameraMap[SM->activateCameraId]->viewMatrix;
	const glm::mat4& perspectiveMatrix = SM->cameraMap[SM->activateCameraId]->perspectiveMatrix;

	GLCall(glUseProgram(shader.GetProgramId()));

	//Buffers
	vb.Bind();
	ib.Bind();

	//Texture
	shader.AddTexture("u_texture", tex);	

	//ModelAttributes
	shader.AddAttrib("a_pos", sizeof(glm::vec3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	shader.AddAttrib("a_uv", sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
	shader.AddAttrib("a_color", sizeof(glm::vec2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

	glm::mat4 SM;
	glm::mat4 TM;
	glm::mat4 modelMatrix;

	//SM = glm::scale(glm::mat4(1.0f), sop.scale);
	TM = glm::translate(glm::mat4(1.0f), sop.translation);

	modelMatrix = TM;

	modelMatrix[0][0] = viewMatrix[0][0];
	modelMatrix[0][1] = viewMatrix[1][0];
	modelMatrix[0][2] = viewMatrix[2][0];
	modelMatrix[1][0] = viewMatrix[0][1];
	modelMatrix[1][1] = viewMatrix[1][1];
	modelMatrix[1][2] = viewMatrix[2][1];
	modelMatrix[2][0] = viewMatrix[0][2];
	modelMatrix[2][1] = viewMatrix[1][2];
	modelMatrix[2][2] = viewMatrix[2][2];

	glm::mat4 LightMatrix = viewMatrix * modelMatrix;
	
	shader.AddMatrix4fv("u_LightMatrix", 1, GL_FALSE, LightMatrix);
	shader.AddMatrix4fv("u_perspectiveMatrix", 1, GL_FALSE, perspectiveMatrix);
	
	//DrawCall
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));

}