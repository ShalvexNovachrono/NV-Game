#include "Keyboard_.h"

int IsAnyKeyDownDEC() {
	for (int Key = 1; Key < 256; Key++) { // ascii dec for character from 1 to 255
		bool KeyPressed = GetAsyncKeyState(Key);
		if (KeyPressed) {
			return Key;
		}
	}
	return 0; // No Key Pressed
}

bool IsThisKeyPressed(char Key) {
	if (GetAsyncKeyState(Key) && 0x8000) {
		return true;
	}
	return false;
}