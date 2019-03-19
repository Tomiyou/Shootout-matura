#include "hud.h"
#include "graphics.h"
#include "globals.h"
#include <stdio.h>
#include "player.h"

HUD::HUD() {}

HUD::HUD(Graphics &graphics, const std::string& filePath, Player* player) :
		_player(player)
{
	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->_spriteSheet == NULL) printf("Error loading file!");

	this->setupItems();
}

HUD::~HUD() {}

void HUD::draw(Graphics& graphics) {
	for (int i = 0; i < this->_player->getHP(); i++) {
		graphics.blitHUDSurface(this->_spriteSheet, &this->_items["HP"].getSourceRect(), &this->_items["HP"].getDestRect(i));
	}
}

void HUD::setupItems() {
	// HP
	SDL_Rect sourceRect = { 0, 0, 200, 200 };
	this->_items["HP"] = HUDitem(sourceRect);
	int x = globals::SCREEN_WIDTH * 0.01;
	int y = globals::SCREEN_HEIGHT * 0.01;
	int width = 100 * globals::HUD_SCALE;
	int height = 100 * globals::HUD_SCALE;
	for (int i = 0; i < this->_player->getHP(); i++) {
		SDL_Rect destRect = { x + width*i + 5, y, width, height };
		this->_items["HP"].addDestRect(destRect);
	}
}
