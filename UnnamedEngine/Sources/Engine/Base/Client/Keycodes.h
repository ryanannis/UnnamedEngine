#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <array>
#include "Engine/Base/Common/Reflection.h"	

typedef int Keycode;

#define KEYMAP(LETTER, KEYCODE) constexpr Key LETTER(#LETTER, KEYCODE)

struct Key
{
	constexpr Key(const char* name, const int keycode) : name(name), keycode(keycode) {}
	const char* name;
	const int keycode;
};

KEYMAP(KEY_A, GLFW_KEY_A);
KEYMAP(KEY_B, GLFW_KEY_B);
KEYMAP(KEY_C, GLFW_KEY_C);
KEYMAP(KEY_D, GLFW_KEY_D);
KEYMAP(KEY_E, GLFW_KEY_E);
KEYMAP(KEY_F, GLFW_KEY_F);
KEYMAP(KEY_G, GLFW_KEY_G);
KEYMAP(KEY_H, GLFW_KEY_H);
KEYMAP(KEY_I, GLFW_KEY_I);
KEYMAP(KEY_J, GLFW_KEY_J);
KEYMAP(KEY_K, GLFW_KEY_K);
KEYMAP(KEY_L, GLFW_KEY_L);
KEYMAP(KEY_M, GLFW_KEY_M);
KEYMAP(KEY_N, GLFW_KEY_N);
KEYMAP(KEY_O, GLFW_KEY_O);
KEYMAP(KEY_P, GLFW_KEY_P);
KEYMAP(KEY_Q, GLFW_KEY_Q);
KEYMAP(KEY_R, GLFW_KEY_R);
KEYMAP(KEY_S, GLFW_KEY_S);
KEYMAP(KEY_T, GLFW_KEY_T);
KEYMAP(KEY_U, GLFW_KEY_U);
KEYMAP(KEY_V, GLFW_KEY_V);
KEYMAP(KEY_W, GLFW_KEY_W);
KEYMAP(KEY_X, GLFW_KEY_X);
KEYMAP(KEY_Y, GLFW_KEY_Y);
KEYMAP(KEY_Z, GLFW_KEY_Z);
KEYMAP(MOUSE_X, 1000);
KEYMAP(MOUSE_Y, 1001);

constexpr auto KEYMAPPINGS = makeArray
(
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z
);

bool operator==(const Key k, const Keycode kc);
bool operator==(const Keycode kc, const Key k);