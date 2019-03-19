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
	Vector2 getNextSpawnpoint();

	bool checkTileCollisions(const Rectangle &other, std::vector<Rectangle> &others);
	Vector2 getMapSize() const;

private:
	std::string _mapName;
	double _spawnpoints[31][2] = { { 6248,673 },{ 6277,1337 },{ 3705,2485 },{ 6069,2437 },{ 4985,2001 },{ 2937,2129 },{ 1041,1429 },{ 455,1800 },{ 2690,3010 },{ 5125,2437 },
		{ 6229,3057 },{ 4293,1345 },{ 5573,893 },{ 938,2784 },{ 2805,1621 },{ 1025,1280 },{ 188,2505 },{ 2453,65 },{ 570,777 },{ 5825,1793 },{ 1783,392 },{ 4745,3077 },{ 445,190 },{ 1221,127 },
		{ 3297,437 },{ 5253,2821 },{ 6246,2422 },{ 1945,2833 },{ 5892,258 },{ 4233,2113 },{ 4805,1260 } };
	//int last_spawns[6];
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
