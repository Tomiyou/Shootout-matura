#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "movement.h"
#include "Interpolator.h"

class Graphics;

class Player : public PlayerEntity {
public:
	Player();
	Player(Graphics &graphics, float x, float y, int id, char * name);
	~Player();
	void draw(Graphics &graphics);
	void update(float &elapsedTime);

	virtual void animationDone(int currentAnimation);
	void setupAnimations();

	// Moves player in direction by "dx" and "dy"
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMoving();

	void handleTileCollisions(std::vector<Rectangle> &others);
	bool bulletCollision(const Rectangle &other);
	void updatePosition(int ticks, int &x, int &y, int &hp);
	int getHP();
private:
	Direction _facing;
	Movement _movement;
	Interpolate _interpolate;

	float _dx, _dy;
};

#endif
