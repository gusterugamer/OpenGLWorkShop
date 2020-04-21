#pragma once
#include <queue>
#include <bitset>

class Keyboard {	
public:
	// To be used in application when needed
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	bool GetKey(unsigned char keycode); // Returns true if the current key if the user holds the key down
	bool GetKeyDown(unsigned char keycode); // Returns true only for the current frame (1 time key stroke)
	bool GetKeyUp(unsigned char keycode); // Returns true when the key has been released
	char GetKeyChar(); //Get the character that the key is linked to

	// Setters for the win32 api when message is dispatched(should have been private and friend linked to window class , but there was no such class)

	void OnKeyPressed(unsigned char keycode); //Sets the state for the current key ( true if is pressed/ false if not)
	void OnKeyReleased(unsigned char keycode); //true if the key was released
	void OnChar(char keycode);//Retrives the character associated with the key
	void ClearState(); //Clear the state of the keys if the window loses focus
private:
	//Maximum number of keys allowed to be pushed;
	static constexpr int nKeys = 256u;
	//maximum number of keys allowed to exist at a time;
	static constexpr int bufferSize = 16u;

	std::bitset<nKeys> keyState;
	std::queue<char> charBuffer; //containar that hold the chars of the keys stroked



};