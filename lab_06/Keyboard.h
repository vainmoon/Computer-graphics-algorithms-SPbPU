#pragma once

#include "KeyboardEvent.h"

#include <queue>

class Keyboard
{
public:
	static Keyboard& getInstance();

	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	KeyboardEvent ReadKey();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);

private:
	Keyboard();
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(Keyboard&) = delete;

private:
	bool autoRepeatKeys = false;
	bool keyStates[256];
	std::queue<KeyboardEvent> keyBuffer;
};

