#include "bullet.h"
#include <cmath>
#include "globals.h"
#include <iostream>

Bullet::Bullet() {}

Bullet::Bullet(Graphics & graphics, int id, float x, float y, float dx, float dy, int parentID) :
	Sprite(graphics, "content/sprites/bullet.png", 0, 0, bullet_constants::FILE_DIMENSIONS, bullet_constants::FILE_DIMENSIONS, 8, 8, x, y),
	id(id),
	_to_be_deleted(false),
	_dx(dx/10000),
	_dy(dy/10000),
	parentID(parentID)
{}

Bullet::~Bullet() {}

void Bullet::update(float elapsedTime) {
	this->_x += this->_dx * elapsedTime;
	this->_y += this->_dy * elapsedTime;

	Sprite::Update();
}

void Bullet::draw(Graphics& graphics) {
	Sprite::draw(graphics);
}
