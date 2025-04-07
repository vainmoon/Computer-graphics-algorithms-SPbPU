#include "Mouse.h"

Mouse& Mouse::getInstance()
{
	static Mouse mouse;
	return mouse;
}

void Mouse::OnLeftPressed(int x, int y)
{
	leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	eventBuffer.push(me);
}

void Mouse::OnLeftReleased(int x, int y)
{
	leftIsDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void Mouse::OnRightPressed(int x, int y)
{
	rightIsDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void Mouse::OnRightReleased(int x, int y)
{
	rightIsDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void Mouse::OnWheelUp(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void Mouse::OnWheelDown(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void Mouse::OnMouseMove(int x, int y)
{
	prevX = curX;
	prevY = curY;
	curX = x;
	curY = y;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

bool Mouse::IsLeftDown()
{
	return leftIsDown;
}

bool Mouse::IsRightDown()
{
	return rightIsDown;
}

MousePoint Mouse::GetPos()
{
	return{ curX, curY };
}

MousePoint Mouse::GetPosChange()
{
	return { curX - prevX, curY - prevY };
}

bool Mouse::EventBufferIsEmpty()
{
	return eventBuffer.empty();
}

MouseEvent Mouse::ReadEvent()
{
	if (eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = eventBuffer.front();
		eventBuffer.pop();
		return e;
	}
}