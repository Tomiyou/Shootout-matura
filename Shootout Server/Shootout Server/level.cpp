#include <SDL2/SDL.h>
#include "graphics.h"
#include "level.h"
#include "tinyxml2.h"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "globals.h"

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Graphics& graphics) :
		_mapName(mapName),
		_size(_size.zero()),
		_backgroundTexture(NULL)
{
	this->loadMap(mapName, graphics);
	unsigned int ticks = std::abs((int)SDL_GetTicks());
	srand(ticks);
}

Level::~Level() {
}

void Level::loadMap(std::string mapName, Graphics &graphics) {
	//Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "content/maps/" << mapName << ".tmx"; //Pass in Map 1, we get maps/Map 1.tmx
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	//Get the width and the height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	//Get the width and the height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	//Loading the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			std::stringstream ss;
			ss << "content/maps/" << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			if (tex == NULL) std::cout << "COULDN'T LOAD MAP!" << std::endl;
			this->_tilesets.push_back(Tileset(tex, firstgid));

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	//Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			//Loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					//Loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//Build each individual tile here
							//If gid is 0, no tile should be drawn. Continue loop
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									//This is the tileset we want
									tls = this->_tilesets.at(i);
									break;
								}
							}

							if (tls.FirstGid == -1) {
								//No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							SDL_Rect finalTilePosition = { xx * globals::RENDER_COEF_HORIZONTAL,
								yy * globals::RENDER_COEF_VERTICAL,
								tileWidth * globals::RENDER_COEF_HORIZONTAL,
								tileHeight * globals::RENDER_COEF_VERTICAL };

							//Calculate the position of the tile in the tileset
							int tilesetWidth, tilesetHeight;
							SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
							int tsxx = gid % (tilesetWidth / tileWidth) - 1;
							tsxx *= tileWidth;
							int tsyy = 0;
							int amt = (gid / (tilesetWidth / tileWidth));
							tsyy = tileHeight * amt;
							SDL_Rect finalTilesetPosition = SDL_Rect{ tsxx, tsyy, tileWidth, tileHeight };

							//Build the actual tile and add it to the level's tile list
							Tile tile(tls.Texture, finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}

			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	//Parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "Collision_wrappers") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						Rectangle wrapper = Rectangle(
							x,
							y,
							width,
							height
						);
						this->_collisionRects.push_back(Pair(wrapper));
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			if (ss.str() == "Collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						for (int k=0; k < this->_collisionRects.size(); k++) {
							if (x >= this->_collisionRects.at(k)._wrapper.getLeft()) {
								if (x + width <= this->_collisionRects.at(k)._wrapper.getRight()) {
									if (y >= this->_collisionRects.at(k)._wrapper.getTop()) {
										if (y + height <= this->_collisionRects.at(k)._wrapper.getBottom()) {
											this->_collisionRects.at(k)._children.push_back(Rectangle(
													std::ceil(x),
													std::ceil(y),
													std::ceil(width),
													std::ceil(height)
											));
										}
									}
								}
							}
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}

	for (unsigned int i = 0; i < this->_collisionRects.size(); i++) {
		this->_collisionRects.at(i)._wrapper = Rectangle(
			std::ceil(this->_collisionRects.at(i)._wrapper.getLeft()),
			std::ceil(this->_collisionRects.at(i)._wrapper.getTop()),
			std::ceil(this->_collisionRects.at(i)._wrapper.getWidth()),
			std::ceil(this->_collisionRects.at(i)._wrapper.getHeight()));
	}
}

void Level::update(float elapsedTime) {}

void Level::draw(Graphics& graphics) {
	for (unsigned int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
}

Vector2 Level::getNextSpawnpoint()
{
	int array_pos = rand() % 31;
	return Vector2(
		this->_spawnpoints[array_pos][0], this->_spawnpoints[array_pos][1]
	);
}

bool Level::checkTileCollisions(const Rectangle &other, std::vector<Rectangle> &others) {
	others.clear();
	bool col = false;

	for (unsigned int k = 0; k < this->_collisionRects.size(); k++) {
		if (this->_collisionRects.at(k)._wrapper.collidesWith(other)) {
			for (unsigned int i = 0; i < this->_collisionRects.at(k)._children.size(); i++) {
				if (this->_collisionRects.at(k)._children.at(i).collidesWith(other)) {
					others.push_back(this->_collisionRects.at(k)._children.at(i));
					col = true;
				}
			}
		}
	}

	return col;
}

Vector2 Level::getMapSize() const {
	int x = this->_size.x * this->_tileSize.x;
	int y = this->_size.y * this->_tileSize.y;
	return Vector2(x, y);
}
