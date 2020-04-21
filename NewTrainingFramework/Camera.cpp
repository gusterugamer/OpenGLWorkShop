#include "stdafx.h"
#include "Camera.h"
#include <iostream>

Camera::Camera(Vector3 position,Vector3 target,GLfloat fov, GLfloat nearZ, GLfloat farZ, float moveSpeed, float rotateSpeed)
	:
	fov(fov),
	nearZ(nearZ),
	farZ(farZ),	
	moveSpeed(moveSpeed),
	rotateSpeed(rotateSpeed),
	position(position),
	target(target)
{	
	R.SetIdentity();
	zAxis = (position - target).Normalize();
	xAxis = UP.Cross(zAxis).Normalize();
	yAxis = zAxis.Cross(xAxis).Normalize();
	perspectiveMatrix.SetPerspective(fov, ratio, nearZ, farZ);	
	UpdateWorldView();	
}

Vector3 Camera::Convert2Vec3(Vector4& Vector4)
{
	return Vector3(Vector4.x, Vector4.y, Vector4.z);
}

const Matrix& Camera::GetPerspectiveMatrix()const
{
	return perspectiveMatrix;
}

const Matrix& Camera::GetViewMatrix() const 
{
	return viewMatrix;
}

const Vector3& Camera::GetPosition() const 
{
	return position;	
}

void Camera::moveOx(float directie)
{	
	xAxis = UP.Cross(zAxis).Normalize();	
	Vector3 vectorDeplasare = xAxis * deltaTime * moveSpeed * directie;
	position += vectorDeplasare;
	target += vectorDeplasare;		
	UpdateWorldView();
}

void Camera::moveOy(int directie)
{	
	yAxis = zAxis.Cross(xAxis).Normalize();	
	Vector3 vectorDeplasare = yAxis * moveSpeed*deltaTime  * (float)directie;
	position += vectorDeplasare;
	target += vectorDeplasare;	
	UpdateWorldView();
}

void Camera::moveOz(int directie)
{	
	zAxis = (position - target).Normalize();	
	Vector3 vectorDeplasare = zAxis*moveSpeed * deltaTime * (float)directie;
	position += vectorDeplasare;
	target += vectorDeplasare;
	UpdateWorldView();
}

void Camera::rotateOx(int directie)
{	
	Matrix rotate;
	rotate.SetRotationX(rotateSpeed * deltaTime * directie);	
	UP = Convert2Vec3(Vector4(0.0f,1.0f,0.0f, 0.0f) * rotate);
	UP = Convert2Vec3(Vector4 (UP, 0.0f)* worldMatrix).Normalize();	
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * rotate;	
	target = Convert2Vec3(rotatedTarget * worldMatrix);	
	zAxis = (position-target).Normalize();	
	yAxis = zAxis.Cross(xAxis).Normalize();
	UpdateWorldView();
}

void Camera::rotateOy(int directie)
{
	Matrix rotate;
	rotate = rotate.SetRotationY(rotateSpeed * deltaTime * directie);	
	Vector4 localTarget = Vector4(0, 0, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * rotate;
	target = Convert2Vec3(rotatedTarget * worldMatrix);	
	zAxis = (position - target).Normalize();
	xAxis = UP.Cross(zAxis).Normalize();	
	UpdateWorldView();
}	

void Camera::rotateOz(int directie)
{
	Matrix rotate;
	rotate = rotate.SetRotationZ(rotateSpeed * deltaTime * directie);
	UP = Convert2Vec3(Vector4(0.0f,1.0f,0.0f, 1.0f) * rotate).Normalize();
	UP = Convert2Vec3(Vector4(UP, 0.0f)*worldMatrix);
	Vector4 localTarget = Vector4(0, 0, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * rotate;
	target = Convert2Vec3(rotatedTarget * worldMatrix);	
	xAxis = UP.Cross(zAxis).Normalize();
	yAxis = zAxis.Cross(xAxis).Normalize();
	UpdateWorldView();	
}

void Camera::Update(ESContext* esContext, const float& deltaTime)
{
	this->deltaTime = deltaTime;		
	if (esContext->kbd.GetKeyUp(0x57))
	{
		moveOz(-1);
	}
	if (esContext->kbd.GetKeyDown(0x53))
	{
		moveOz(1);
	}
	if (esContext->kbd.GetKey(0x41))
	{
		moveOx(-1);		
	}	
	if (esContext->kbd.GetKey(0x44))
	{
		moveOx(1);
	}
	if (esContext->kbd.GetKey(0x5A))
	{
		moveOy(1);
	}
	if (esContext->kbd.GetKey(0x58))
	{
		moveOy(-1);
	}
	if (esContext->kbd.GetKey(VK_LEFT))
	{
		rotateOy(1);
	}
	if (esContext->kbd.GetKey(VK_RIGHT))
	{
		rotateOy(-1);
	}
	if (esContext->kbd.GetKey(VK_UP))
	{
		rotateOx(-1);
	}
	if (esContext->kbd.GetKey(VK_DOWN))
	{
		rotateOx(1);
	}
	if (esContext->kbd.GetKey(VK_NUMPAD1))
	{
		rotateOz(1);
	}
	if (esContext->kbd.GetKey(VK_NUMPAD2))
	{
		rotateOz(-1);
	}	
}

void Camera::UpdateWorldView()
{
	R.SetIdentity();
	R.m[0][0] = xAxis.x;
	R.m[0][1] = xAxis.y;
	R.m[0][2] = xAxis.z;
	R.m[1][0] = yAxis.x;
	R.m[1][1] = yAxis.y;
	R.m[1][2] = yAxis.z;
	R.m[2][0] = zAxis.x;
	R.m[2][1] = zAxis.y;
	R.m[2][2] = zAxis.z;	

	T=T.SetTranslation(position);

	worldMatrix = R * T;
	
	T=T.SetTranslation(-position);
	R = R.Transpose();

	viewMatrix = T*R;	
}