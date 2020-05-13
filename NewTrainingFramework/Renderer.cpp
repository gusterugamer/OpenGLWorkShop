#include "stdafx.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "DebugModeFunctions.h"
#include <string>

void Renderer::Draw(const SceneObjectProperties& sop, const Model& model, Shader& shader, const Texture& tex)
{
	const Matrix& viewMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& perspectiveMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetPerspectiveMatrix();

	GLCall(glUseProgram(shader.GetProgramId()));	

	//Buffers
	model.GetVd().Bind();
	model.GetIb().Bind();
	
	//Texture
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glUniform1i(shader.textureUniform, 0));
	tex.Bind();

	//ModelAttributes
	GLCall(glEnableVertexAttribArray(shader.positionAttribute));
	GLCall(glVertexAttribPointer(shader.positionAttribute, sizeof(Vector3)/sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));

	GLint normalLoc = shader.AddAttrib("a_normal");
	GLCall(glEnableVertexAttribArray(normalLoc));
	GLCall(glVertexAttribPointer(normalLoc, sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, norm)));
	
	/*GLint tanLoc = shader.AddAttrib("a_tan");
	GLCall(glEnableVertexAttribArray(tanLoc));
	GLCall(glVertexAttribPointer(normalLoc, sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, tgt)));

	GLint binormLoc = shader.AddAttrib("a_binorm");
	GLCall(glEnableVertexAttribArray(binormLoc));
	GLCall(glVertexAttribPointer(normalLoc, sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, binorm)));*/

	GLCall(glEnableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glVertexAttribPointer(shader.textureCoordAttrib, sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv)));

	GLint ambientalLoc = shader.AddUniform("material.ambient");
	GLint diffuseLoc = shader.AddUniform("material.diffuse");
	GLint specularLoc = shader.AddUniform("material.specular");
	GLint shininessLoc = shader.AddUniform("material.shininess");

	GLCall(glUniform3f(ambientalLoc, sop.mat.ambiental.x, sop.mat.ambiental.y, sop.mat.ambiental.z));
	GLCall(glUniform3f(diffuseLoc, sop.mat.diffuse.x, sop.mat.diffuse.y, sop.mat.diffuse.z));
	GLCall(glUniform3f(specularLoc, sop.mat.specular.x, sop.mat.specular.y, sop.mat.specular.z));
	GLCall(glUniform1f(shininessLoc, sop.mat.shininess));
	
	//Light

	//Object properties related to light reflection	

	//GLcall(glUniform3f(lightStructLoc))

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
	TM = TM.SetTranslation((Vector3)sop.translation);
	modelMatrix = SM * TM * RM;


	GLCall(glUniformMatrix4fv(shader.modelMatrixUniform, 1, GL_FALSE, (GLfloat*)modelMatrix.m));
	GLCall(glUniformMatrix4fv(shader.viewMatrixUniform, 1, GL_FALSE, (GLfloat*)viewMatrix.m));
	GLCall(glUniformMatrix4fv(shader.perspectiveMatrixUniform, 1, GL_FALSE, (GLfloat*)perspectiveMatrix.m));	
	//DrawCall
	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));

	GLCall(glDisableVertexAttribArray(normalLoc));
	/*GLCall(glDisableVertexAttribArray(tanLoc));
	GLCall(glDisableVertexAttribArray(binormLoc));*/
	GLCall(glDisableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glDisableVertexAttribArray(shader.positionAttribute));
	
}

void Renderer::DrawBlendedTextures(const Matrix& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const Matrix& viewMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& perspectiveMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetPerspectiveMatrix();	

	GLCall(glUseProgram(shader.GetProgramId()));

	model.GetVd().Bind();
	model.GetIb().Bind();
	
	for (unsigned int i = 0; i < tex.size(); i++)
	{		
		std::string s = "u_texture[" + std::to_string(i) + "]" ;
		GLCall(glActiveTexture(GL_TEXTURE0+i));
		GLCall(glUniform1i(shader.AddUniform(s.c_str()),i));
		tex[i]->Bind();
	}	

	GLCall(glEnableVertexAttribArray(shader.positionAttribute));
	GLCall(glVertexAttribPointer(shader.positionAttribute, sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));

	GLCall(glEnableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glVertexAttribPointer(shader.textureCoordAttrib, sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv)));	

	GLint attribBlendUv = shader.AddAttrib("a_uvblend");
	GLCall(glEnableVertexAttribArray(attribBlendUv));
	GLCall(glVertexAttribPointer(attribBlendUv, sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uvblend)));

	GLCall(glUniformMatrix4fv(shader.viewMatrixUniform, 1, GL_FALSE, (GLfloat*)viewMatrix.m));
	GLCall(glUniformMatrix4fv(shader.perspectiveMatrixUniform, 1, GL_FALSE, (GLfloat*)perspectiveMatrix.m));
	GLCall(glUniformMatrix4fv(shader.modelMatrixUniform, 1, GL_FALSE, (GLfloat*)modelMatrix.m));

	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));
	
	GLCall(glDisableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glDisableVertexAttribArray(shader.positionAttribute));
	GLCall(glDisableVertexAttribArray(attribBlendUv));
}

void Renderer::DrawBlendedTextures(const Matrix& modelMatrix, const VertexBuffer& vb, const IndexBuffer& ib, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const Matrix& viewMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& perspectiveMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetPerspectiveMatrix();

	GLCall(glUseProgram(shader.GetProgramId()));

	vb.Bind();
	ib.Bind();

	for (unsigned int i = 0; i < tex.size(); i++)
	{
		std::string s = "u_texture[" + std::to_string(i) + "]";
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
		GLCall(glUniform1i(shader.AddUniform(s.c_str()), i));
		tex[i]->Bind();
	}

	GLCall(glEnableVertexAttribArray(shader.positionAttribute));
	GLCall(glVertexAttribPointer(shader.positionAttribute, sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));

	GLCall(glEnableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glVertexAttribPointer(shader.textureCoordAttrib, sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv)));

	GLint attribBlendUv = shader.AddAttrib("a_uvblend");
	GLCall(glEnableVertexAttribArray(attribBlendUv));
	GLCall(glVertexAttribPointer(attribBlendUv, sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uvblend)));

	GLCall(glUniformMatrix4fv(shader.viewMatrixUniform, 1, GL_FALSE, (GLfloat*)viewMatrix.m));
	GLCall(glUniformMatrix4fv(shader.perspectiveMatrixUniform, 1, GL_FALSE, (GLfloat*)perspectiveMatrix.m));
	GLCall(glUniformMatrix4fv(shader.modelMatrixUniform, 1, GL_FALSE, (GLfloat*)modelMatrix.m));

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));

	GLCall(glDisableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glDisableVertexAttribArray(shader.positionAttribute));
}

void Renderer::DrawMultiTexture(const Matrix& modelMatrix, Model& model, Shader& shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const Matrix& viewMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& perspectiveMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetPerspectiveMatrix();

	GLCall(glUseProgram(shader.GetProgramId()));

	model.GetVd().Bind();
	model.GetIb().Bind();

	for (unsigned int i = 0; i < tex.size(); i++)
	{
		std::string s = "u_texture[" + std::to_string(i) + "]";
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
		GLCall(glUniform1i(shader.AddUniform(s.c_str()), i));
		tex[i]->Bind();
	}

	GLCall(glEnableVertexAttribArray(shader.positionAttribute));
	GLCall(glVertexAttribPointer(shader.positionAttribute, sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));

	GLCall(glEnableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glVertexAttribPointer(shader.textureCoordAttrib, sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv)));

	GLCall(glUniformMatrix4fv(shader.viewMatrixUniform, 1, GL_FALSE, (GLfloat*)viewMatrix.m));
	GLCall(glUniformMatrix4fv(shader.perspectiveMatrixUniform, 1, GL_FALSE, (GLfloat*)perspectiveMatrix.m));
	GLCall(glUniformMatrix4fv(shader.modelMatrixUniform, 1, GL_FALSE, (GLfloat*)modelMatrix.m));

	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));

	GLCall(glDisableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glDisableVertexAttribArray(shader.positionAttribute));	
}

void Renderer::DrawSkyBox(const Matrix& modelMatrix, Model& model, Shader& shader, const Texture& tex)
{
	const Matrix& viewMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& perspectiveMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetPerspectiveMatrix();

	GLCall(glUseProgram(shader.GetProgramId()));

	model.GetVd().Bind();
	model.GetIb().Bind();

	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glUniform1i(shader.AddUniform("u_texture_cube"), 0));
	tex.Bind();

	GLCall(glEnableVertexAttribArray(shader.positionAttribute));
	GLCall(glVertexAttribPointer(shader.positionAttribute, sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_TRUE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));

	GLCall(glUniformMatrix4fv(shader.viewMatrixUniform, 1, GL_FALSE, (GLfloat*)viewMatrix.m));
	GLCall(glUniformMatrix4fv(shader.perspectiveMatrixUniform, 1, GL_FALSE, (GLfloat*)perspectiveMatrix.m));
	GLCall(glUniformMatrix4fv(shader.modelMatrixUniform, 1, GL_FALSE, (GLfloat*)modelMatrix.m));

	GLCall(glDrawElements(GL_TRIANGLES, model.GetIb().GetCount(), GL_UNSIGNED_SHORT, nullptr));
	
	GLCall(glDisableVertexAttribArray(shader.positionAttribute));
}

void Renderer::DrawDebug(const SceneObjectProperties& sop, const VertexBuffer& vb, const IndexBuffer& ib, Shader& shader, const Texture& tex)
{
	const Matrix& viewMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& perspectiveMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetPerspectiveMatrix();

	GLCall(glUseProgram(shader.GetProgramId()));

	//Buffers
	vb.Bind();
	ib.Bind();

	//Texture
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glUniform1i(shader.textureUniform, 0));
	tex.Bind();

	//ModelAttributes
	GLCall(glEnableVertexAttribArray(shader.positionAttribute));
	GLCall(glVertexAttribPointer(shader.positionAttribute, sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));

	GLCall(glEnableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glVertexAttribPointer(shader.textureCoordAttrib, sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv)));

	GLint colorLoc = shader.AddAttrib("a_color");
	GLCall(glEnableVertexAttribArray(colorLoc));
	GLCall(glVertexAttribPointer(colorLoc, sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color)));

	Matrix SM;
	Matrix TM;
	Matrix modelMatrix;
	
	SM = SM.SetScale((Vector3)sop.scale);
	TM = TM.SetTranslation((Vector3)sop.translation);

	modelMatrix = SM * TM ;

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

	GLCall(glUniformMatrix4fv(shader.AddUniform("u_LightMatrix"), 1, GL_FALSE, (GLfloat*)LightMatrix.m));
	GLCall(glUniformMatrix4fv(shader.AddUniform("u_PerspectiveMatrix"), 1, GL_FALSE, (GLfloat*)perspectiveMatrix.m));
	//DrawCall
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));

	GLCall(glDisableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glDisableVertexAttribArray(shader.positionAttribute));
	GLCall(glDisableVertexAttribArray(colorLoc));

}