#include "input.h"

void Input::beginNewFrame() {
	this->_releasedKeys.clear();
	this->_pressedKeys.clear();
	this->_releasedButtons.clear();
	this->_pressedButtons.clear();
}

// KEYBOARD

void Input::keyDownEvent(const SDL_Event& event) {
	this->_pressedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = true;
}

void Input::keyUpEvent(const SDL_Event& event) {
	this->_releasedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = false;
}

bool Input::isKeyHeld(SDL_Scancode key) {
	return this->_heldKeys[key];
}

bool Input::wasKeyPressed(SDL_Scancode key) {
	return this->_pressedKeys[key];
}

bool Input::wasKeyReleased(SDL_Scancode key) {
	return this->_releasedKeys[key];
}

// MOUSE

void Input::buttonDownEvent(const SDL_Event& event) {
	this->_pressedButtons[event.button.button] = Click(event.button.x, event.button.y);
	this->_heldButtons[event.button.button] = true;
}

void Input::buttonUpEvent(const SDL_Event& event) {
	this->_releasedButtons[event.button.button] = true;
	this->_heldButtons[event.button.button] = false;
}

Click* Input::wasButtonPressed(Uint8 button) {
	Click *test = NULL;
	if (!this->_pressedButtons[button].getDefault()) {
		test = &this->_pressedButtons[button];
	}
	return test;
}

bool Input::wasButtonReleased(Uint8 button) {
	return this->_releasedButtons[button];
}

bool Input::isButtonHeld(Uint8 button) {
	return this->_heldButtons[button];
}
