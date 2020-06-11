#pragma once
#include "Globals.h"
#include "../Utilities/Mathematics.h"
#include "../Utilities/glm/glm.hpp"

class Camera {
	friend class Renderer;
	friend class SceneManager;
private:	
    const GLfloat ratio = (GLfloat)(Globals::screenWidth / Globals::screenHeight);
	GLfloat moveSpeed;
	GLfloat rotateSpeed;
	GLfloat nearZ;
	GLfloat farZ;
	GLfloat fov;
	GLfloat deltaTime;
	
	glm::vec3 position;
	glm::vec3 target;	
	glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 left = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 xAxis;	
	glm::vec3 zAxis;
	glm::vec3 yAxis;		

	glm::mat4 viewMatrix;
	glm::mat4 worldMatrix;
	glm::mat4 perspectiveMatrix;	
	glm::mat4 T;
	glm::mat4 R;	

	std::pair<int, int> startMousePos;
	std::pair<int, int> point;
public:

	Camera() = delete;
	Camera(glm::vec3& position, glm::vec3& target, GLfloat fov, GLfloat nearZ, GLfloat farZ, float moveSpeed, float rotateSpeed);

	void moveOx(float directie);
	void moveOy(float directie);
	void moveOz(float directie);
	void rotateOx(char directie);
	void rotateOy(char directie);
	void rotateOz(char directie);
	void Update(ESContext* esContext, const float& deltaTime);	
	void FixedUpdate(ESContext* esContext, const float& deltaTime);
	void UpdateWorldView();	

    const glm::vec3& GetPosition() const;

	const glm::mat4& GetperspectiveMatrix() const;
	const glm::mat4& GetviewMatrix() const;	

	///TEMPORAR///
	void QuatRotationY(int mousePos);
	void QuatRotationX(int mousePos);
	///TEMPORAR///
};