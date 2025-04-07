#pragma once

#include <Windows.h>

#include <string>

class RenderWindow
{
public:
	RenderWindow(const HINSTANCE& hInstance, const int& width, const int& height, const std::wstring& windowClass, const std::wstring& title);
	~RenderWindow();

	bool Init(int nCmdShow);
	bool ProcessMessages();

	HWND getHwnd();
	int getWidth();
	int getHeight();

	void Resize(const int& width, const int& height);

private:
	void RegisterWindowClass();

private:
	HWND hwnd = NULL;
	HINSTANCE hInstance = NULL;
	int width = 0;
	int height = 0;
	std::wstring windowClass = L"";
	std::wstring title = L"";
};

