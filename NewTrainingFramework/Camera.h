#pragma once
#include "Globals.h"
#include "../Utilities/Math.h"

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
	
	Vector3 position;
	Vector3 target;	
	Vector3 UP = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 xAxis;	
	Vector3 zAxis;
	Vector3 yAxis;		

	Matrix viewMatrix;
	Matrix worldMatrix;
	Matrix perspectiveMatrix;	
	Matrix T;
	Matrix R;	
public:

	Camera() = delete;
	Camera(Vector3 position, Vector3 target, GLfloat fov, GLfloat nearZ, GLfloat farZ, float moveSpeed, float RotateSpeed);

	void moveOx(char directie);
	void moveOy(char directie);
	void moveOz(char directie);
	void rotateOx(char directie);
	void rotateOy(char directie);
	void rotateOz(char directie);
	void Update(ESContext* esContext, const float& deltaTime);	
	void UpdateWorldView();	

    const Vector3& GetPosition() const;
	Vector3 Convert2Vec3(Vector4 & vector4);	

	const Matrix& GetPerspectiveMatrix() const;
	const Matrix& GetViewMatrix() const;	
};