#include "stdafx.h"
#include "Keyboard.h"

bool Keyboard::GetKey(unsigned char keycode)
{
	return keyState[keycode];
}

bool Keyboard::GetKeyDown(unsigned char keycode)
{
	if (keyState[keycode] && !keyStateDown[keycode])
	{
		keyStateDown[keycode] = true;
		return true;
	}
	return false;
}

bool Keyboard::GetKeyUp(unsigned char keycode)
{
	if (!keyState[keycode] && keyStateDown[keycode])
	{
		keyStateDown[keycode] = false;
		return true;
	}
	return false;
}

char Keyboard::GetKeyChar()
{
	return keyChar;
}

void Keyboard::OnKeyPressed(unsigned char keycode)
{	
	keyState[keycode] = true;	
	keyStateDown[keycode] = false;
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	keyState[keycode] = false;	
	keyStateDown[keycode] = true;
}

void Keyboard::OnChar(char character)
{
	keyChar = character;
}

void Keyboard::ClearState()
{
	keyState.reset();
	keyStateDown.reset();
}
