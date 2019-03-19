#ifndef TEXT_H
#define TEXT_H

#include <SDL2\SDL_ttf.h>

class Graphics;

class Text {
public:
	Text();
	Text(Graphics & graphics, char * text, int width, int height);
	~Text();

	void update(float & x, float & y);
	void draw(Graphics & graphics);
private:
	TTF_Font* _font;
	SDL_Color _color;
	SDL_Texture* _name;
	SDL_Rect _destinationRect;
};


#endif