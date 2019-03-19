#include "camera.h"
#include <math.h>
#include <iostream>

Camera::Camera() {};

Camera::Camera(Vector2 playerSize, Vector2 mapSize) :
		_x(0),
		_y(0)
{
	this->_margin = Vector2(floor((globals::SCREEN_WIDTH - playerSize.x) / 2), floor((globals::SCREEN_HEIGHT - playerSize.y) / 2));
	this->_mapSize = Vector2(mapSize.x * globals::WORLD_SCALE, mapSize.y * globals::WORLD_SCALE);
}

void Camera::update(float & x, float & y) {
	this->_x = x * globals::WORLD_SCALE - this->_margin.x;
	this->_y = y * globals::WORLD_SCALE - this->_margin.y;

	if (this->getLeft() < 0)
		this->_x = 0;
	else if (this->getRight() > this->_mapSize.x)
		this->_x = this->_mapSize.x - globals::SCREEN_WIDTH;

	if (this->_y < 0)
		this->_y = 0;
	else if (this->getBottom() > this->_mapSize.y)
		this->_y = this->_mapSize.y - globals::SCREEN_HEIGHT;
}

int Camera::getLeft() const {
	return this->_x;
}

int Camera::getRight() const {
	return this->_x + globals::SCREEN_WIDTH;
}

int Camera::getTop() const {
	return this->_y;
}

int Camera::getBottom() const {
	return this->_y + globals::SCREEN_HEIGHT;
}

int Camera::getX() const {
	return this->_x;
}

int Camera::getY() const {
	return this->_y;
}
