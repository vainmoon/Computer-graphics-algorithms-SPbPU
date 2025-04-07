#include "Keyboard.h"

Keyboard& Keyboard::getInstance() 
{
	static Keyboard keyboard;
	return keyboard;
}

Keyboard::Keyboard()
{
	for (int i = 0; i < 256; i++)
		keyStates[i] = false; //Initialize all key states to off (false)
}

bool Keyboard::KeyIsPressed(const unsigned char keycode)
{
	return keyStates[keycode];
}

bool Keyboard::KeyBufferIsEmpty()
{
	return keyBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey()
{
	if (keyBuffer.empty()) //If no keys to be read?
	{
		return KeyboardEvent(); //return empty keyboard event
	}
	else
	{
		KeyboardEvent e = keyBuffer.front(); //Get first Keyboard Event from queue
		keyBuffer.pop(); //Remove first item from queue
		return e; //Returns keyboard event
	}
}

void Keyboard::OnKeyPressed(const unsigned char key)
{
	keyStates[key] = true;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void Keyboard::OnKeyReleased(const unsigned char key)
{
	keyStates[key] = false;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}