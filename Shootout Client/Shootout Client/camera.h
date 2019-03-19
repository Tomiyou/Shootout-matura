#ifndef CAMERA_H
#define CAMERA_H

#include "globals.h"

class Camera {
public:
	Camera();
	Camera(Vector2 playerSize, Vector2 mapSize);
	void update(float & player_x, float & player_y);

	int getLeft() const;
	int getRight() const;
	int getTop() const;
	int getBottom() const;

	int getX() const;
	int getY() const;
private:
	int _x, _y;
	Vector2 _mapSize, _margin;

};

#endif
