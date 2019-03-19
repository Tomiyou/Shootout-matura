#include "hp_bar.h"
#include "globals.h"
#include <iostream>
#include "graphics.h"

HP_bar::HP_bar() {}

HP_bar::HP_bar(Graphics & graphics, const std::string & filePath, int width, int height) : 
	_current_frame(0),
	_offset(width)
{
	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->_spriteSheet == NULL)
		std::cout << "Error loading file!" << std::endl;

	for (unsigned int i = 0; i < 11; i++) {
		SDL_Rect new_rect = { i * width, 0, width, height };
		this->_frames.push_back(new_rect);
	}

	this->_destination_rectangle.w = width * globals::WORLD_SCALE;
	this->_destination_rectangle.h = height * globals::WORLD_SCALE;
}

HP_bar::~HP_bar() {

}

void HP_bar::setHP(int hp) {
	float new_hp = 10.0f * (1.0f - hp / 10.0f) + 0.5f;

	if (new_hp > 10)
		this->_current_frame = 10;
	else
		this->_current_frame = new_hp;
}

void HP_bar::update(float &x, float &y) {
	// VPRASAJ ZA HITROST: funkcija() vs spremenljivka
	this->_destination_rectangle.x = x;
	this->_destination_rectangle.y = y + this->_offset;
}

void HP_bar::draw(Graphics & graphics) {
	this->_destination_rectangle.x *= globals::WORLD_SCALE;
	this->_destination_rectangle.y *= globals::WORLD_SCALE;
	graphics.blitSurface(this->_spriteSheet, &this->_frames[this->_current_frame], &this->_destination_rectangle);
}
