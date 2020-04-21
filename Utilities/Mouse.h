#pragma once
#include <windef.h>
#include <bitset>

class Mouse
{
public:
	enum class MouseBtns {
		none=-1,
		LeftButton=1,
		RightButton=2,
		MiddleButton=4,
		WheelUp=5,
		WheelDown=6
	};

public:
	//Function that the user uses
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator= (const Mouse&) = delete;

	//Function that the user uses
	bool IsMouseButtonPressed(unsigned int BtnCode) noexcept;
	bool IsMouseButtonDown(unsigned int BtnCode) noexcept;
	bool IsMouseButtonUp(unsigned int BtnCode) noexcept;
	int getX() noexcept;
	int getY() noexcept;
	std::pair<int, int> getPosition() noexcept;

	//Functions that take the information from Win32 API
	void OnMouseMove(int x, int y);
	void OnMouseButtonPressed(MouseBtns btn, int x, int y);
	void OnMouseButtonReleased(MouseBtns btn, int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void ClearState();
private:
	int x;
	int y;
	static constexpr int nKeys = 256u;
	std::bitset<nKeys> keyState;
	std::bitset<nKeys> keyStateDown;
};