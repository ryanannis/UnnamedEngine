#pragma once

enum class InputState {
	INPUT_UP,
	INPUT_DOWN
};

typedef int Keycode;

class InputEvent
{
public:
	InputEvent(Keycode keycode, InputState state);
	~InputEvent();

	Keycode GetKeycode() const { return(mKeycode); }
	InputState GetState() const { return(mState); }

private:
	Keycode mKeycode;
	InputState mState;
};