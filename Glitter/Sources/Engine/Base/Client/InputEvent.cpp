#include "InputEvent.h"

InputEvent::InputEvent(Keycode keycode, InputState state) :
	mKeycode(keycode),
	mState(state)
{

}

InputEvent::~InputEvent()
{
}