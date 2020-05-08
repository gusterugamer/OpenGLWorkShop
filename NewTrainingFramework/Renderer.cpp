#include "stdafx.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "OpenGLErrorChecking.h"
#include <string>

void Renderer::Draw(const SceneObjectProperties& sop, const Model& model, Shader& shader, const Texture& tex, Light& light)
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
	
	//Light

	//Object properties related to light reflection
	GLint reflectLoc = shader.AddUniform("u_reflectivity");
	GLint shineDumpLoc = shader.AddUniform("u_shineDamper");
	GLCall(glUniform1f(reflectLoc, sop.reflectivity));
	GLCall(glUniform1f(shineDumpLoc, sop.shineDamper));

	//Sending camera position to shader for reflection calculation
	GLint cameraPosLoc = shader.AddUniform("u_cameraPos");
	GLCall(glUniform3f(cameraPosLoc, SceneManager::GetInstance()->GetCurrentCamera()->GetPosition().x, 
									 SceneManager::GetInstance()->GetCurrentCamera()->GetPosition().y,
									 SceneManager::GetInstance()->GetCurrentCamera()->GetPosition().z));

	//LightColor
	GLint lightColorDifLoc = shader.AddUniform("u_lightColorDif");
	GLCall(glUniform3f(lightColorDifLoc, light.getColorDif().x, light.getColorDif().y, light.getColorDif().z));	
	
	GLint lightColorSpecLoc = shader.AddUniform("u_lightColorSpec");
	GLCall(glUniform3f(lightColorDifLoc, light.getColorSpec().x, light.getColorSpec().y, light.getColorSpec().z));

	//LightPosition
	GLint lightPosLoc = shader.AddUniform("u_lightPosition");
	GLCall(glUniform3f(lightPosLoc, light.getPosition().x, light.getPosition().y, light.getPosition().z));

	GLint lightAmbStructLoc =  shader.AddUniform("pLight.ambient");
	GLint lightDiffStructLoc = shader.AddUniform("pLight.diffuse");
	GLint lightSpecStructLoc = shader.AddUniform("pLight.specular");
	GLint lightDirSturctLoc =  shader.AddUniform("pLight.direction");
	GLint lightPositionSturctLoc =  shader.AddUniform("pLight.position");
	GLint lightColorSturctLoc =shader.AddUniform("pLight.color");
	GLint lightConstSturctLoc =shader.AddUniform("pLight.constant");
	GLint lightLinearSturctLoc = shader.AddUniform("pLight.linear");
	GLint lightQuadraSturctLoc = shader.AddUniform("pLight.quadratic");
	GLint lightcutOffSturctLoc = shader.AddUniform("cutOff");
	GLint lightoutCutOFFSturctLoc = shader.AddUniform("outterCutOff");

												  
	GLint materialAmbStructLoc = shader.AddUniform("material.ambient");
	GLint materialDiffStructLoc = shader.AddUniform("material.diffuse");
	GLint materialSpecStructLoc = shader.AddUniform("material.specular");
	GLint materialShinStructLoc = shader.AddUniform("material.shininess");

	GLCall(glUniform3f(lightAmbStructLoc, 1.0f, 1.0f, 1.0f));
	GLCall(glUniform3f(lightDiffStructLoc,1.0f, 1.0f, 1.0f));
	GLCall(glUniform3f(lightSpecStructLoc,1.0f, 1.0f, 1.0f));
	GLCall(glUniform3f(lightDirSturctLoc, 0.0f, 0.0f, 1.0f));
	GLCall(glUniform3f(lightPositionSturctLoc, 0.0f, 0.0f, -122.0f));
	GLCall(glUniform3f(lightColorSturctLoc, 1.0f, 1.0f, 1.0f));
	GLCall(glUniform1f(lightConstSturctLoc, 1.0f));
	GLCall(glUniform1f(lightLinearSturctLoc, 0.014f));
	GLCall(glUniform1f(lightQuadraSturctLoc, 0.0007f));
	GLCall(glUniform1f(lightcutOffSturctLoc , 0.91f));
	GLCall(glUniform1f(lightoutCutOFFSturctLoc, 0.82f));

	GLCall(glUniform3f(materialAmbStructLoc, 1.0f, 1.0f, 1.0f));
	GLCall(glUniform3f(materialDiffStructLoc, 1.0f, 1.0f, 1.0f));
	GLCall(glUniform3f(materialSpecStructLoc, 1.0f, 1.0f, 1.0f));
	GLCall(glUniform1f(materialShinStructLoc, 1.0f));

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




