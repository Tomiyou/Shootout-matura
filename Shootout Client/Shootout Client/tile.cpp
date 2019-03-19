#include <SDL2/SDL.h>
#include "tile.h"
#include "graphics.h"

Tile::Tile() {}

Tile::Tile(SDL_Texture* tileset, SDL_Rect sourceRect, SDL_Rect destinationRect) :
	_tileset(tileset),
	_sourceRect(sourceRect),
	_destinationRect(destinationRect),
	_orgDestRect(destinationRect)
{}

void Tile::update(int elapsedTime) {
}

void Tile::draw(Graphics& graphics) {
	this->_destinationRect.x = this->_orgDestRect.x;
	this->_destinationRect.y = this->_orgDestRect.y;
	graphics.blitSurface(this->_tileset, &this->_sourceRect, &this->_destinationRect);
}
