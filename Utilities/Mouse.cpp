#include "stdafx.h"
#include "Mouse.h"

bool Mouse::IsMouseButtonPressed(unsigned int BtnCode) noexcept
{
	return keyState[BtnCode];
}

bool Mouse::IsMouseButtonDown(unsigned int BtnCode) noexcept
{
	if (keyState[BtnCode] && !keyStateDown[BtnCode])
	{
		keyStateDown[BtnCode] = true;
		return true;
	}
	return false;
}

bool Mouse::IsMouseButtonUp(unsigned int BtnCode) noexcept
{
	if (!keyState[BtnCode] && keyStateDown[BtnCode])
	{
		keyStateDown[BtnCode] = false;
		return true;
	}
	return false;
}

int Mouse::getX() noexcept
{
	return x;
}

int Mouse::getY() noexcept
{
	return y;
}

std::pair<int, int> Mouse::getPosition() noexcept
{
	return std::pair<int, int>(x,y);	
}

void Mouse::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Mouse::OnMouseButtonPressed(MouseBtns btn, int x, int y)
{
	keyState[(unsigned int)btn] = true;
	keyStateDown[(unsigned int)btn] = false;
	this->x = x;
	this->y = y;
}

void Mouse::OnMouseButtonReleased(MouseBtns btn, int x, int y)
{
	keyState[(unsigned int)btn] = false;
	keyStateDown[(unsigned int)btn] = true;
	this->x = x;
	this->y = y;
}

void Mouse::OnWheelUp(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Mouse::OnWheelDown(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Mouse::ClearState()
{
	keyState.reset();
	keyStateDown.reset();
}


