#pragma once

enum InputState {
	INPUT_UP,
	INPUT_DOWN
};

class InputEvent
{
public:
	InputEvent();
	~InputEvent();

private:
	int mKeycode;
	InputState mState;
};