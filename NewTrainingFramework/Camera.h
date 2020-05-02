#pragma once
#include "Globals.h"
#include "../Utilities/Math.h"

class Camera {	
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

	ESContext* esContext;	
public:

	Camera() = delete;
	Camera(Vector3 position, Vector3 target, GLfloat fov, GLfloat nearZ, GLfloat farZ, float moveSpeed, float RotateSpeed);

	void moveOx(float directie);
	void moveOy(int directie);
	void moveOz(int directie);
	void rotateOx(float directie);
	void rotateOy(float directie);
	void rotateOz(int directie);
	void Update(ESContext* esContext, const float& deltaTime);	
	void UpdateWorldView();	

    const Vector3& GetPosition() const;
	Vector3 Convert2Vec3(Vector4 & vector4);	

	const Matrix& GetPerspectiveMatrix() const;
	const Matrix& GetViewMatrix() const;	
};