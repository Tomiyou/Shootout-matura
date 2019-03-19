#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include "globals.h"
#include <vector>
#include "tile.h"
#include "rectangle.h"

class Graphics;
struct SDL_Texture;
struct Tileset;

struct Pair {
	Rectangle _wrapper;
	std::vector<Rectangle> _children;

	Pair() {};
	Pair(Rectangle wrapper) :
		_wrapper(wrapper),
		_children()
	{}
};

class Level {
public:
	Level();
	Level(std::string mapName, Graphics &graphics);
	~Level();

	void update(float elapsedTime);
	void draw(Graphics &graphics);

	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	Vector2 getMapSize() const;

private:
	std::string _mapName;
	Vector2 _size;
	Vector2 _tileSize;
	SDL_Texture* _backgroundTexture;
	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Pair> _collisionRects;

	void loadMap(std::string mapName, Graphics &graphics);

};

struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;

	Tileset() :	FirstGid(-1)
	{}

	Tileset(SDL_Texture* texture, int firstGid) :
		Texture(texture),
		FirstGid(firstGid)
	{}
};

#endif
