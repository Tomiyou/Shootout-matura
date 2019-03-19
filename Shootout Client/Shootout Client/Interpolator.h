#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include "globals.h"

class Interpolate {
public:
	Interpolate();
	~Interpolate();

	int update(int ticks, int x, int y);
	void quadratic_solve(int elapsedTime, float& _x, float& _y);
	void quadratic_player_solve(int elapsedTime, float& _x, float& _y);
	void linear_solve(int elapsedTime, float& _x, float& _y);
	void prediction_fixMove(float& _x, float& _y);
	void hardmove(int x, int y);
private:
	InterpolVec _positions[3];
	float _timestep, _elapsedTime;
};

#endif