#include "stdafx.h"
#include <windows.h>
#include "esUtil.h"
#include <chrono>
#include <cmath>
#include <iostream>

static constexpr float fixedDeltaTime = 0.1f;


// Main window procedure
LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	ESContext* esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
	LRESULT  lRet = 1;
	switch (uMsg)
	{
		
	case WM_CREATE:
		break;	

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		//*******************************KEYBOARDMESSAGES*******************************//
	case WM_KEYDOWN:
		//Added by trainee
	case WM_SYSKEYDOWN:
	{
		//if (esContext && esContext->keyFunc)              
		//    esContext->keyFunc(esContext, (unsigned char)wParam, true);
			//Added by trainee
		if (esContext)
		{
			if (!(lParam & 0x40000000))
			{
				esContext->kbd.OnKeyPressed(static_cast<unsigned char> (wParam));
			}
		}
	}

	break;

	case WM_KEYUP:
		//Added by trainee
	case WM_SYSKEYUP:
	{
		//if ( esContext && esContext->keyFunc )
		   // esContext->keyFunc ( esContext, (unsigned char) wParam, false );
		//Added by trainee
		if (esContext)
		{
			esContext->kbd.OnKeyReleased(static_cast<unsigned char> (wParam));
		}
	}
	//Added by trainee

	break;
	//Added by trainee
	case WM_CHAR:
	{		
		if (esContext)
		{
			esContext->kbd.OnChar(static_cast<unsigned char> (wParam));
		}
	}
	break;
	//*******************************KEYBOARDMESSAGES*******************************//

	//*******************************MOUSEMESSAGES**********************************//
  //Added by trainee
	case WM_LBUTTONDOWN:
	{		
		if (esContext)
		{
			POINTS pt = MAKEPOINTS(lParam);
			esContext->mouse.OnMouseButtonPressed(Mouse::MouseBtns::LeftButton, pt.x, pt.y);
		}
	}
	break;
	//Added by trainee
	case WM_LBUTTONUP:
	{		
		if (esContext)
		{
			POINTS pt = MAKEPOINTS(lParam);
			esContext->mouse.OnMouseButtonReleased(Mouse::MouseBtns::LeftButton, pt.x, pt.y);
		}
	}
	break;
	//Added by trainee
	case WM_RBUTTONDOWN:
	{		
		if (esContext)
		{
			POINTS pt = MAKEPOINTS(lParam);
			esContext->mouse.OnMouseButtonPressed(Mouse::MouseBtns::RightButton, pt.x, pt.y);
		}
	}
	break;
	//Added by trainee
	case WM_RBUTTONUP:
	{		
		if (esContext)
		{
			POINTS pt = MAKEPOINTS(lParam);
			esContext->mouse.OnMouseButtonReleased(Mouse::MouseBtns::RightButton, pt.x, pt.y);
		}
	}
	//Added by trainee
	break;
	case WM_MBUTTONDOWN:
	{	
		if (esContext)
		{
			POINTS pt = MAKEPOINTS(lParam);
			esContext->mouse.OnMouseButtonPressed(Mouse::MouseBtns::MiddleButton, pt.x, pt.y);
		}
	}
	break;
	//Added by trainee
	case WM_MBUTTONUP:
	{		
		if (esContext)
		{
			POINTS pt = MAKEPOINTS(lParam);
			esContext->mouse.OnMouseButtonReleased(Mouse::MouseBtns::MiddleButton, pt.x, pt.y);
		}
	}
	break;

	//Added by trainee
	case WM_MOUSEWHEEL:
	{		
		if (esContext)
		{
			POINTS pt = MAKEPOINTS(lParam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				esContext->mouse.OnWheelUp(pt.x, pt.y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				esContext->mouse.OnWheelDown(pt.x, pt.y);
			}
		}
	}
	break;
	//Added by trainee
	case WM_MOUSEMOVE:
	{		
		if (esContext)
		{
			POINTS pt = MAKEPOINTS(lParam);
			esContext->mouse.OnMouseMove(pt.x,pt.y);
		}
	}

	break;
	//*******************************MOUSEMESSAGES**********************************//
	//Added by trainee
	case WM_KILLFOCUS:
	{		
		if (esContext)
		{
			esContext->kbd.ClearState();
			esContext->mouse.ClearState();
		}
	}
	break;


	default:
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}

//      Create Win32 instance and window
GLboolean WinCreate(ESContext* esContext, const char* title)
{
	WNDCLASS wndclass = { 0 };
	DWORD    wStyle = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);


	wndclass.style = CS_OWNDC;
	wndclass.lpfnWndProc = (WNDPROC)ESWindowProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclass.lpszClassName = "opengles2.0";

	if (!RegisterClass(&wndclass))
		return FALSE;

	wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = esContext->width;
	windowRect.bottom = esContext->height;

	AdjustWindowRect(&windowRect, wStyle, FALSE);

	esContext->hWnd = CreateWindow(
		"opengles2.0",
		title,
		wStyle,
		0,
		0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Set the ESContext* to the GWL_USERDATA so that it is available to the 
	// ESWindowProc
	if (esContext->hWnd == NULL)
	{
		MessageBox(NULL, "Can not create window!", "Error", 0);
		return -1;
	}
	SetWindowLongPtr(esContext->hWnd, GWL_USERDATA, (LONG)(LONG_PTR)esContext);


	if (esContext->hWnd == NULL)
		return GL_FALSE;

	ShowWindow(esContext->hWnd, TRUE);

	return GL_TRUE;
}



//      Start main windows loop
void WinLoop(ESContext* esContext)
{
	MSG msg = { 0 };	
	int done = 0;
	auto timeNow = std::chrono::high_resolution_clock::now();
	float fixedTimePassed = 0.0f;

	while (!done)
	{	
		int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
		auto timeEnd = std::chrono::high_resolution_clock::now();		
		std::chrono::duration<float> timePast = timeEnd - timeNow;		
		float deltaTime = timePast.count();
		fixedTimePassed += deltaTime;
		timeNow = timeEnd;		

		if (gotMsg)
		{
			if (msg.message == WM_QUIT)
			{
				done = 1;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}			
		}
		else
		{
			if (esContext && esContext->drawFunc != NULL)
				esContext->drawFunc(esContext);
		}
		if (esContext && esContext->updateFunc != NULL)
			esContext->updateFunc(esContext, deltaTime);		
		
		if (fixedTimePassed >= fixedDeltaTime)
		{
			fixedTimePassed = 0.0f;
			if (esContext && esContext->updateFunc != NULL)
				esContext->fixedUpdateFunc(esContext, fixedDeltaTime);
		}

		//Removed(commented) by trainee	

		//   // Call Update function	
		//if (esContext->drawFunc != NULL)
		//	esContext->drawFunc(esContext);

			
	}
}
