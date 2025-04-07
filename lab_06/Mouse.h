#pragma once

#include "MouseEvent.h"
#include <queue>

class Mouse
{
public:
	static Mouse& getInstance();

	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);

	bool IsLeftDown();
	bool IsRightDown();

	MousePoint GetPos();
	MousePoint GetPosChange();

	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();

private:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(Mouse&) = delete;

private:
	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool rightIsDown = false;
	int curX = 0;
	int curY = 0;
	int prevX = 0;
	int prevY = 0;
};

