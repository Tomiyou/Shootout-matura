#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "movement.h"
#include <vector>
#include "sprite.h"

class Graphics;

class Player : public Sprite {
public:
	Player();
	Player(Graphics &graphics, float x, float y, int id);
	~Player();
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	// Moves player in direction by "dx" and "dy"
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMoving();

	void handleTileCollisions(std::vector<Rectangle> &others);

	int getHP() const;
	bool bulletCollision(const Rectangle &other, const int &id);
	void die(Vector2 &newSpawn);

	char* getName() {
		return this->_name;
	}
	Player* getPointer() {
		return this;
	}
	int& getState();
private:
	Rectangle getLegacyBoundingBox() const;
	Direction _facing;
	Movement _movement;
	int _lives, _dead;
	Vector2 _colArray[5], _newSpawn;

	char _name[16];
};

#endif
