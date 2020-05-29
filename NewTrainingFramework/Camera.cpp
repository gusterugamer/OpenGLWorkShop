#include "stdafx.h"
#include "Camera.h"
#include "../Utilities/glm/gtc/quaternion.hpp"
#include "../Utilities/glm/gtx/quaternion.hpp"
#include <iostream>

Camera::Camera(glm::vec3& position, glm::vec3& target,GLfloat fov, GLfloat nearZ, GLfloat farZ, float moveSpeed, float rotateSpeed)
	:
	fov(fov),
	nearZ(nearZ),
	farZ(farZ),	
	moveSpeed(moveSpeed),
	rotateSpeed(rotateSpeed),
	position(position),
	target(target)
{		
	perspectiveMatrix = glm::perspective(fov, ratio, nearZ, farZ);	
	UpdateWorldView();	
}

const glm::mat4& Camera::GetperspectiveMatrix()const
{
	return perspectiveMatrix;
}

const glm::mat4& Camera::GetviewMatrix() const
{
	return viewMatrix;
}

void Camera::QuatRotationY(int mousePos)
{
	glm::quat Quaternion = glm::angleAxis(glm::radians((float)mousePos * 10.0f) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));	

	left = glm::vec3(Quaternion * glm::vec4(left, 1.0f));
	forward = glm::vec3(Quaternion * glm::vec4(forward, 1.0f));
	UP = glm::vec3(Quaternion * glm::vec4(UP, 1.0f));
	target = glm::vec3(Quaternion * glm::vec4(target, 1.0f));
	
	UpdateWorldView();
}

void Camera::QuatRotationX(int mousePos)
{	
	glm::quat Quaternion = glm::angleAxis(glm::radians((float)mousePos*10.0f) * deltaTime,left);	
	
	UP = glm::vec3(Quaternion * glm::vec4(UP,1.0f));
	left = glm::vec3(Quaternion * glm::vec4(left, 1.0f));
	forward = glm::vec3(Quaternion * glm::vec4(forward, 1.0f));
	target = glm::vec3(Quaternion * glm::vec4(target,1.0f));
	UpdateWorldView();
}

const glm::vec3& Camera::GetPosition() const 
{
	return position;	
}

void Camera::moveOx(float directie)
{	
	//xAxis = UP.Cross(zAxis).Normalize();	
	glm::vec3 vectorDeplasare = left * deltaTime * moveSpeed * directie;
	position += vectorDeplasare;
	target += vectorDeplasare;		
	UpdateWorldView();
}

void Camera::moveOy(float directie)
{	
	//yAxis = zAxis.Cross(xAxis).Normalize();	
	glm::vec3 vectorDeplasare = UP * moveSpeed*deltaTime  * (float)directie;
	position += vectorDeplasare;
	target += vectorDeplasare;	
	UpdateWorldView();
}

void Camera::moveOz(float directie)
{	
	//zAxis = (position - target).Normalize();	
	glm::vec3 vectorDeplasare = forward *moveSpeed * deltaTime * (float)directie;
	position += vectorDeplasare;
	target += vectorDeplasare;
	UpdateWorldView();
}

void Camera::rotateOx(char directie)
{	
	//QuatRotation(directie);
//	glm::mat4 rotate;
//	rotate.SetRotationX(directie * deltaTime * rotateSpeed);	
//	UP = Convert2Vec3(glm::vec4(0.0f,1.0f,0.0f, 0.0f) * rotate);
//	UP = Convert2Vec3(glm::vec4 (UP, 0.0f)* worldMatrix).Normalize();	
//	glm::vec4 localTarget = glm::vec4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
//	glm::vec4 rotatedTarget = localTarget * rotate;	
//	target = Convert2Vec3(rotatedTarget * worldMatrix);	
//	//zAxis = (position-target).Normalize();	
////	yAxis = zAxis.Cross(xAxis).Normalize();
//	
//	UpdateWorldView();
}

void Camera::rotateOy(char directie)
{
	//glm::mat4 rotate;
	//rotate = rotate.SetRotationY(rotateSpeed * deltaTime * directie);	
	//rotate = rotate.SetRotationY(directie * deltaTime * rotateSpeed);
	//glm::vec4 localTarget = glm::vec4(0, 0, -(target - position).Length(), 1.0f);
	//glm::vec4 rotatedTarget = localTarget * rotate;
	//target = Convert2Vec3(rotatedTarget * worldMatrix);	
	////zAxis = (position - target).Normalize();
	////xAxis = UP.Cross(zAxis).Normalize();	
	UpdateWorldView();
}	

void Camera::rotateOz(char directie)
{
	/*glm::mat4 rotate;
	rotate = rotate.SetRotationZ(rotateSpeed * deltaTime * directie);
	UP = Convert2Vec3(glm::vec4(0.0f,1.0f,0.0f, 1.0f) * rotate).Normalize();
	UP = Convert2Vec3(glm::vec4(UP, 0.0f)*worldMatrix);
	glm::vec4 localTarget = glm::vec4(0, 0, -(target - position).Length(), 1.0f);
	glm::vec4 rotatedTarget = localTarget * rotate;
	target = Convert2Vec3(rotatedTarget * worldMatrix);	*/
	//xAxis = UP.Cross(zAxis).Normalize();
	//yAxis = zAxis.Cross(xAxis).Normalize();
	UpdateWorldView();	
}

void Camera::Update(ESContext* esContext, const float& deltaTime)
{	
	this->deltaTime = deltaTime;	
	if (esContext->kbd.GetKey(0x57))
	{
		moveOz(1);
	}	
	if (esContext->kbd.GetKey(0x53))
	{
		moveOz(-1);
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
		rotateOx(1);
	}
	if (esContext->kbd.GetKey(VK_DOWN))
	{
		rotateOx(-1);
	}
	if (esContext->kbd.GetKey(VK_NUMPAD1))
	{
		rotateOz(1);
	}
	if (esContext->kbd.GetKey(VK_NUMPAD2))
	{
		rotateOz(-1);
	}	
	if (esContext->mouse.IsMouseButtonDown(VK_RBUTTON))
	{
		startMousePos = esContext->mouse.getPosition();
	}
	if (esContext->mouse.IsMouseButtonPressed(VK_RBUTTON))
	{			
		point = esContext->mouse.getPosition();	
		int dx = startMousePos.first - point.first;
		int dy = startMousePos.second - point.second;		
		QuatRotationY(dx);	
		QuatRotationX(dy);		
		startMousePos = point;
	}	
}

void Camera::UpdateWorldView()
{	
	viewMatrix = glm::lookAtRH(position,position+forward,UP);	
}
