#include "stdafx.h"
#include "SceneManager.h"
#include "OpenGLErrorChecking.h"
#include <string>

void Renderer::Draw(const VertexBuffer & vb,const IndexBuffer & ib,Shader & shader,const Texture& tex)
{
	const Matrix& viewMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& perspectiveMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetPerspectiveMatrix();

	GLCall(glUseProgram(shader.GetProgramId()));	

	vb.Bind();
	ib.Bind();	
	
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glUniform1i(shader.textureUniform, 0));
	tex.Bind();

	GLCall(glEnableVertexAttribArray(shader.positionAttribute));
	GLCall(glVertexAttribPointer(shader.positionAttribute, sizeof(Vector3)/sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));

	GLCall(glEnableVertexAttribArray(shader.textureCoordAttrib));
	GLCall(glVertexAttribPointer(shader.textureCoordAttrib, sizeof(Vector2) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv)));

	GLCall(glUniformMatrix4fv(shader.viewMatrixUniform, 1, GL_FALSE, (GLfloat*)viewMatrix.m));
	GLCall(glUniformMatrix4fv(shader.perspectiveMatrixUniform, 1, GL_FALSE, (GLfloat*)perspectiveMatrix.m));

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));	
}

void Renderer::DrawTerrain(const VertexBuffer & vb, const IndexBuffer & ib, Shader & shader, const std::vector<std::shared_ptr<Texture>>& tex)
{
	const Matrix& viewMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& perspectiveMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetPerspectiveMatrix();	

	GLCall(glUseProgram(shader.GetProgramId()));

	vb.Bind();
	ib.Bind();
	
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
	

	GLCall(glUniformMatrix4fv(shader.viewMatrixUniform, 1, GL_FALSE, (GLfloat*)viewMatrix.m));
	GLCall(glUniformMatrix4fv(shader.perspectiveMatrixUniform, 1, GL_FALSE, (GLfloat*)perspectiveMatrix.m));

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));	
}

void Renderer::DrawSkyBox(const VertexBuffer & vb, const IndexBuffer & ib, Shader & shader, const Texture& tex)
{
	const Matrix& viewMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetViewMatrix();
	const Matrix& perspectiveMatrix = SceneManager::GetInstance()->GetCurrentCamera()->GetPerspectiveMatrix();

	GLCall(glUseProgram(shader.GetProgramId()));

	vb.Bind();
	ib.Bind();


	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glUniform1i(shader.AddUniform("u_texture_cube"), 0));
	tex.Bind();

	GLCall(glEnableVertexAttribArray(shader.positionAttribute));
	GLCall(glVertexAttribPointer(shader.positionAttribute, sizeof(Vector3) / sizeof(GLfloat), GL_FLOAT, GL_TRUE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));

	GLCall(glUniformMatrix4fv(shader.viewMatrixUniform, 1, GL_FALSE, (GLfloat*)viewMatrix.m));
	GLCall(glUniformMatrix4fv(shader.perspectiveMatrixUniform, 1, GL_FALSE, (GLfloat*)perspectiveMatrix.m));

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_SHORT, nullptr));
}




