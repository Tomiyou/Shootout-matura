#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <map>
#include "click.h"

class Input {
public:
	void beginNewFrame();
	void keyUpEvent(const SDL_Event& event);
	void keyDownEvent(const SDL_Event& event);
	void buttonUpEvent(const SDL_Event& event);
	void buttonDownEvent(const SDL_Event& event);

	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);

	Click* wasButtonPressed(Uint8 button);
	bool wasButtonReleased(Uint8 button);
	bool isButtonHeld(Uint8 button);
private:
	std::map<SDL_Scancode, bool> _heldKeys;
	std::map<SDL_Scancode, bool> _pressedKeys;
	std::map<SDL_Scancode, bool> _releasedKeys;

	std::map<Uint8, bool> _heldButtons;
	std::map<Uint8, Click> _pressedButtons;
	std::map<Uint8, bool> _releasedButtons;
};

#endif
