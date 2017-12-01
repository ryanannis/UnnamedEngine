#pragma once

enum class InputState {
	INPUT_UP,
	INPUT_DOWN,
	MOUSE_MOVEMENT
};

typedef int Keycode;

class InputEvent
{
public:
	InputEvent(Keycode keycode, InputState state, float value = 0.0f);
	~InputEvent();

	Keycode GetKeycode() const { return(mKeycode); }
	InputState GetState() const { return(mState); }
	float GetValue() { return(mValue);  }

private:
	Keycode mKeycode;
	InputState mState;
	float mValue;
};
