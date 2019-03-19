#ifndef BULLET_H
#define BULLET_H

#include "sprite.h"
#include "globals.h"
#include "Interpolator.h"

namespace bullet_constants {
	const int FILE_DIMENSIONS = 12;
	const float BULLET_SPEED = 2.0f;
}

class Graphics;

class Bullet : public Sprite {
public:
	Bullet();
	Bullet(Graphics &graphics, int id, float x, float y, float dx, float dy, int parentID);
	~Bullet();

	void update(float elapsedTime);
	void draw(Graphics &graphics);

	int getID() const {
		return id;
	}
	int getParentID() const {
		return parentID;
	}

	bool _to_be_deleted;
private:
	int id, parentID;
	float _dx, _dy;
};

#endif

