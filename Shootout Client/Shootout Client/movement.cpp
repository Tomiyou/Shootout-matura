#include "movement.h"
#include <cmath>

namespace {
	const double PI = 3.14159265359;
}

Movement::Movement() {};

Movement::Movement(double speed) :
		_x(0),
		_y(0),
		_speed(speed),
		_dx(0),
		_dy(0)
{
	this->_rootSpeed = speed / sqrt(2);
}

Movement::~Movement() {
}

void Movement::move(float x, float y) {
	this->_x += x;
	this->_y += y;

	int x_sign = this->_x / std::abs(this->_x);
	int y_sign = this->_y / std::abs(this->_y);

	if (this->_y != 0) {
		if (this->_x == 0)
			this->_dy = this->_speed * y_sign;
		else
			this->_dy = this->_rootSpeed * y_sign;
	}
	else this->_dy = 0;

	if (this->_x != 0) {
		if (this->_y == 0)
			this->_dx = this->_speed * x_sign;
		else
			this->_dx = this->_rootSpeed * x_sign;
	}
	else this->_dx = 0;
}

double Movement::getdX() const {
	return this->_dx;
}

double Movement::getdY() const {
	return this->_dy;
}
