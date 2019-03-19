#include "text.h"
#include "graphics.h"
#include "globals.h"

Text::Text() {}

Text::Text(Graphics & graphics, char * text, int width, int height)
{
	this->_font = TTF_OpenFont("Consolas.ttf", 24);
	this->_color = { 0, 0, 0 };
	SDL_Surface* graphic_text = TTF_RenderText_Solid(this->_font, text, this->_color);
	this->_name = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphic_text);
	SDL_FreeSurface(graphic_text);

	this->_destinationRect.w = width * globals::WORLD_SCALE;
	this->_destinationRect.h = height * globals::WORLD_SCALE;
}

Text::~Text() {

}

void Text::update(float & x, float & y) {
	this->_destinationRect.x = x;
	this->_destinationRect.y = y - this->_destinationRect.h;
}

void Text::draw(Graphics & graphics) {
	this->_destinationRect.x *= globals::WORLD_SCALE;
	this->_destinationRect.y *= globals::WORLD_SCALE;
	graphics.blitTextSurface(this->_name, &this->_destinationRect);
}
