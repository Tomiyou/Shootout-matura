#ifndef BULLET_H
#define BULLET_H

#include "sprite.h"
#include "globals.h"

namespace bullet_constants {
	const int FILE_DIMENSIONS = 4;
	const float BULLET_SPEED = 2.0f;
}

class Graphics;

class Bullet : public Sprite {
public:
	Bullet();
	Bullet(Graphics &graphics, Rectangle &player_box, float dx, float dy, int id, int parent_id);
	~Bullet();

	int update(float elapsedTime);
	void draw(Graphics &graphics);

	int getParentID() const;
	Rectangle getLegacyBoundingBox() const;

	float getDX() const {
		return this->_dx;
	}
	float getDY() const {
		return this->_dy;
	}
private:
	float _dx, _dy;
	int _parentID;
	Vector2 _colArray [5];
};

#endif

