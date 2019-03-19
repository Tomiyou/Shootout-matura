#ifndef HUD_H
#define HUD_H

#include <SDL2/SDL.h>
#include <string>
#include <map>
#include <vector>

class Graphics;
class Player;

struct HUDitem {
public:
	HUDitem() {};
	HUDitem(SDL_Rect sourceRect) :
		_sourceRect(sourceRect)
	{}

	SDL_Rect getSourceRect() const {
		return _sourceRect;
	}
	SDL_Rect getDestRect(int i) const {
		return this->_destRects.at(i);
	}

	void addDestRect(SDL_Rect rectangle) {
		this->_destRects.push_back(rectangle);
	}
private:
	SDL_Rect _sourceRect;
	std::vector<SDL_Rect> _destRects;
};

class HUD {
public:
	HUD();
	HUD(Graphics &graphics, const std::string &filePath, Player* player);
	~HUD();

	void draw(Graphics& graphics);
	void setupItems();
private:
	Player* _player;
	SDL_Texture* _spriteSheet;
	std::map<std::string, HUDitem> _items;
};

#endif
