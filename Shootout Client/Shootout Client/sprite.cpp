#include "sprite.h"
#include "graphics.h"
#include "globals.h"
#include <stdio.h>
#include <cmath>
#include <iostream>

Sprite::Sprite() {}

Sprite::Sprite(Graphics& graphics, const std::string &filePath, int sourceX, int sourceY, int sourceW, int sourceH, int width, int height, float posX, float posY) :
		_x(posX),
		_y(posY)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = sourceW;
	this->_sourceRect.h = sourceH;
	this->_destinationRect.x = this->_x;
	this->_destinationRect.y = this->_y;
	this->_destinationRect.w = width * globals::WORLD_SCALE;
	this->_destinationRect.h = height * globals::WORLD_SCALE;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->_spriteSheet == NULL) printf("Error loading file!");

	this->_boundingBox = Rectangle(this->_x, this->_y, width, height);
}

Sprite::~Sprite() {}

void Sprite::Update() {
	this->_boundingBox.setPosition(this->_x, this->_y);
	this->_destinationRect.x = this->_x * globals::WORLD_SCALE;
	this->_destinationRect.y = this->_y * globals::WORLD_SCALE;
}

void Sprite::draw(Graphics& graphics) {
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &this->_destinationRect);
}

const Rectangle Sprite::getBoundingBox() const {
	return this->_boundingBox;
}

//Side getCollisionSide
//Determine which side the collision happened on
const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = this->getBoundingBox().getRight() - other.getLeft();
	amtLeft = other.getRight() - this->getBoundingBox().getLeft();
	amtTop = other.getBottom() - this->getBoundingBox().getTop();
	amtBottom = this->getBoundingBox().getBottom() - other.getTop();

	int vals[4] = { std::abs(amtRight), std::abs(amtLeft), std::abs(amtTop), std::abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 0; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}

	return
			lowest == std::abs(amtRight) ? sides::RIGHT :
			lowest == std::abs(amtLeft) ? sides::LEFT :
			lowest == std::abs(amtTop) ? sides::TOP :
			lowest == std::abs(amtBottom) ? sides::BOTTOM :
			sides::NONE;

}

Vector2 Sprite::getSize() const {
	return Vector2(this->_destinationRect.w, this->_destinationRect.h);
}

float& Sprite::getX() {
	return this->_x;
}

float& Sprite::getY() {
	return this->_y;
}
