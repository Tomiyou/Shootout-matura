#include "Interpolator.h"
#include <iostream>

Interpolate::Interpolate() :
	_timestep(0),
	_elapsedTime(0)
{}

Interpolate::~Interpolate() {}

int Interpolate::update(int ticks, int x, int y)
{
	this->_positions[2] = this->_positions[1];
	this->_positions[1] = this->_positions[0];
	this->_positions[0] = InterpolVec(x, y, ticks);

	this->_timestep = this->_positions[0].ticks - this->_positions[2].ticks;
	this->_elapsedTime = 0;


	int d_x = this->_positions[2].x - this->_positions[0].x;
	int d_y = this->_positions[2].y - this->_positions[0].y;

	if (d_x == 0) {
		if (d_y == 0)
			return 0;
		else if (d_y < 0)
			return 2;
		else
			return 4;
	}
	else {
		if (d_x < 0)
			return 3;
		else
			return 1;
	}
}

void Interpolate::quadratic_solve(int elapsedTime, float& _x, float& _y)
{
	if (this->_positions[2].ticks == -1)
		return;

	this->_elapsedTime += elapsedTime;
	float t = this->_elapsedTime / this->_timestep;

	_x = (1 - t) * (1 - t) * this->_positions[2].x + 2 * (1 - t) * t * this->_positions[1].x + t*t * this->_positions[0].x;

	_y = (1 - t) * (1 - t) * this->_positions[2].y +
		2 * (1 - t) * t * this->_positions[1].y +
		t*t * this->_positions[0].y;
}

void Interpolate::quadratic_player_solve(int elapsedTime, float & _x, float & _y)
{
	if (this->_positions[2].ticks == -1)
		return;

	this->_elapsedTime += elapsedTime;
	float t = this->_elapsedTime / this->_timestep + 0.5f;
	std::cout << t << std::endl;

	_x = (1 - t) * (1 - t) * this->_positions[2].x + 2 * (1 - t) * t * this->_positions[1].x + t*t * this->_positions[0].x;

	_y = (1 - t) * (1 - t) * this->_positions[2].y +
		2 * (1 - t) * t * this->_positions[1].y +
		t*t * this->_positions[0].y;
}

void Interpolate::linear_solve(int elapsedTime, float& _x, float& _y)
{
	if (this->_positions[2].ticks == -1)
		return;

	this->_elapsedTime += elapsedTime;
	float t = this->_elapsedTime / this->_timestep;

	// (1–t) * P2 + t * P1
	_x = (1 - t) * this->_positions[2].x + t * this->_positions[0].x;
	_y = (1 - t) * this->_positions[2].y + t * this->_positions[0].y;

}

void Interpolate::prediction_fixMove(float & _x, float & _y) {
	if (this->_positions[0].ticks == -1)
		return;

	_x = this->_positions[0].x;
	_y = this->_positions[0].y;
}

void Interpolate::hardmove(int x, int y) {
	for (int i = 0; i < 3; i++) {
		this->_positions[i].x = x;
		this->_positions[i].y = y;
	}
}


