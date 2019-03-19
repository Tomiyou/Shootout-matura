#include "bullet.h"
#include <cmath>
#include "globals.h"
#include <iostream>

Bullet::Bullet() {}

Bullet::Bullet(Graphics& graphics, Rectangle &player_box, float dx, float dy, int id, int parent_id) :
		Sprite(graphics, "content/sprites/bullet.png", 0, 0, 8, 8, 0, 0, id),
		_dx(dx),
		_dy(dy),
		_parentID(parent_id)
{
	double radians = std::atan2(dx, dy);
	this->_dx = bullet_constants::BULLET_SPEED * std::sin(radians);	
	this->_dy = bullet_constants::BULLET_SPEED * std::cos(radians);

	Vector2 size = this->getSize();
	this->_x = player_box.getCenterX() - size.x / 2;
	this->_y = player_box.getCenterY() - size.y / 2;
}

Bullet::~Bullet() {
}

int Bullet::update(float elapsedTime) {
	this->_x += this->_dx * elapsedTime;
	this->_y += this->_dy * elapsedTime;

	// Update collision array
	this->_colArray[0] = this->getPosition();
	for (int i=4; i > 0; i--)
		this->_colArray[i] = this->_colArray[i-1];


	Sprite::Update();
	return 0;
}

void Bullet::draw(Graphics& graphics) {
	Sprite::draw(graphics);
}

int Bullet::getParentID() const {
	return this->_parentID;
}

Rectangle Bullet::getLegacyBoundingBox() const {
	Rectangle box = Rectangle(this->_colArray[4].x, this->_colArray[4].y,
			this->_sourceRect.w, this->_sourceRect.h);
	return box;
}
