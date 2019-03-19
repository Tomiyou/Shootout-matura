#include "sprite.h"
#include "graphics.h"
#include "globals.h"
#include <stdio.h>
#include <cmath>

Sprite::Sprite() {}

Sprite::Sprite(Graphics& graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY, int id) :
		_x(posX),
		_y(posY),
		_entityID(id)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;
	this->_destinationRect.x = this->_x;
	this->_destinationRect.y = this->_y;
	this->_destinationRect.w = width * globals::RENDER_COEF_HORIZONTAL;
	this->_destinationRect.h = height * globals::RENDER_COEF_VERTICAL;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->_spriteSheet == NULL) printf("Error loading file!");

	this->_boundingBox = Rectangle(this->_x, this->_y, width, height);
}

Sprite::~Sprite() {}

void Sprite::Update() {
	this->_boundingBox.setPosition(this->_x, this->_y);
}

void Sprite::draw(Graphics& graphics) {
	this->_destinationRect.x = this->_x * globals::RENDER_COEF_HORIZONTAL;
	this->_destinationRect.y = this->_y * globals::RENDER_COEF_VERTICAL;
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
	return Vector2(this->_sourceRect.w, this->_sourceRect.h);
}

Vector2 Sprite::getPosition() const {
	return Vector2(this->_x, this->_y);
}

const int Sprite::getID() const {
	return this->_entityID;
}
