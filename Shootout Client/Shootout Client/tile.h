#ifndef TILE_H
#define TILE_H

#include "globals.h"

class Graphics;
class SDL_Texture;

class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, SDL_Rect sourceRect, SDL_Rect destinationRect);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
private:
	SDL_Texture* _tileset;
	Vector2 _size;
	SDL_Rect _sourceRect, _destinationRect, _orgDestRect;
};

#endif
