#include <SDL2/SDL.h>
#include "tile.h"
#include "graphics.h"
#include <iostream>

Tile::Tile() {}

Tile::Tile(SDL_Texture* tileset, SDL_Rect sourceRect, SDL_Rect destinationRect) :
	_tileset(tileset)
{
	this->_sourceRect = sourceRect;
	this->_destinationRect = destinationRect;
}

void Tile::update(int elapsedTime) {
}

void Tile::draw(Graphics& graphics) {
	graphics.blitSurface(this->_tileset, &this->_sourceRect, &this->_destinationRect);
}
