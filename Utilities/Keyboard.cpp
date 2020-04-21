#include "stdafx.h"
#include "Keyboard.h"

bool Keyboard::GetKey(unsigned char keycode)
{
	return keyState[keycode];
}

bool Keyboard::GetKeyDown(unsigned char keycode)
{
	return false;
}

bool Keyboard::GetKeyUp(unsigned char keycode)
{
	return false;
}

char Keyboard::GetKeyChar()
{
	if (charBuffer.size() > 0u)
	{
		unsigned char charcode = charBuffer.front();
		charBuffer.pop();
		return charcode;
	}
}

void Keyboard::OnKeyPressed(unsigned char keycode)
{	
	keyState[keycode] = true;	
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
	keyState[keycode] = false;	
}

void Keyboard::OnChar(char character)
{
	/*charBuffer.push(character);
	while (charBuffer.size() > bufferSize)
		charBuffer.pop();*/
}

void Keyboard::ClearState()
{
	keyState.reset();
}
