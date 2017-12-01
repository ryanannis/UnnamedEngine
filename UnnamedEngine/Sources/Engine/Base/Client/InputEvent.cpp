#include "InputEvent.h"

InputEvent::InputEvent(Keycode keycode, InputState state, float value) :
	mKeycode(keycode),
	mState(state),
	mValue(value)
{
}

InputEvent::~InputEvent()
{
}
