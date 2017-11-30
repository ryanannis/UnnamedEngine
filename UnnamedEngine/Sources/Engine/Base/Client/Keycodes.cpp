#include "Keycodes.h"

bool operator==(const Key k, const Keycode kc)
{
	return(kc == k.keycode);
}

bool operator==(const Keycode kc, const Key k)
{
	return(kc == k.keycode);
}