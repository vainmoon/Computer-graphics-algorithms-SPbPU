#include "RenderWindow.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Engine.h"

RenderWindow::RenderWindow(const HINSTANCE& hInstance, const int& width, const int& height, const std::wstring& windowClass, const std::wstring& title)
    : hInstance(hInstance), width(width), height(height), windowClass(windowClass), title(title)
{
    RegisterWindowClass();
}

RenderWindow::~RenderWindow()
{
	if (hwnd != NULL)
	{
		UnregisterClass(windowClass.c_str(), this->hInstance);
		DestroyWindow(hwnd);
	}
}

bool RenderWindow::Init(int nCmdShow)
{
	hwnd = CreateWindowW(windowClass.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
	{
		return FALSE;
	}

	RECT rc;
	rc.left = 0;
	rc.right = width;
	rc.top = 0;
	rc.bottom = height;

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);

	MoveWindow(hwnd, 0, 0, rc.right - rc.left, rc.bottom - rc.top, TRUE);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	return true;
}

bool RenderWindow::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, 
		hwnd, 
		0,    
		0,    
		PM_REMOVE))
	{
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(hwnd))
		{
			hwnd = NULL;
			UnregisterClass(windowClass.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

HWND RenderWindow::getHwnd()
{
	return hwnd;
}

int RenderWindow::getWidth()
{
	return width;
}

int RenderWindow::getHeight()
{
	return height;
}

void RenderWindow::Resize(const int& width, const int& height)
{
	this->width = width;
	this->height = height;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		Keyboard::getInstance().OnKeyPressed(keycode);
		
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		Keyboard::getInstance().OnKeyReleased(keycode);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::getInstance().OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::getInstance().OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::getInstance().OnRightPressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::getInstance().OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::getInstance().OnRightReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			Mouse::getInstance().OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			Mouse::getInstance().OnWheelDown(x, y);
		}
		return 0;
	}
	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		Engine::getInstance().WindowResize(width, height);

		break;
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = this->hInstance; 
	wc.hIcon = NULL;   
	wc.hIconSm = NULL; 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL; 
	wc.lpszMenuName = NULL; 
	wc.lpszClassName = windowClass.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}
