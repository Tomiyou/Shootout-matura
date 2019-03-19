#include "player.h"
#include "graphics.h"
#include "globals.h"
#include <iostream>

namespace player_constants {
	const float WALK_SPEED = 0.4f;
	// Location of spriteSheet on disk
	const std::string PLAYER_SS = "content/sprites/player.png";
}

Player::Player() {}

Player::Player(Graphics& graphics, float x, float y, int id) :
	Sprite(graphics, player_constants::PLAYER_SS, 0, 0, 58, 58, x, y, id),
	_facing(RIGHT),
	_movement(player_constants::WALK_SPEED),
	_lives(10),
	_dead(-1)
{}

Player::~Player() {}

void Player::moveLeft() {
	this->_movement.move(-1, 0);
}

void Player::moveRight() {
	this->_movement.move(1, 0);
}

void Player::moveUp() {
	this->_movement.move(0, -1);
}

void Player::moveDown() {
	this->_movement.move(0, 1);
}

void Player::stopMoving() {
	//this->_dx = 0.0f;
}

void Player::update(float elapsedTime) {
	if (this->_dead == -1) {

		// Move the player
		this->_x += this->_movement.getdX() * elapsedTime;
		this->_y += this->_movement.getdY() * elapsedTime;

		// Update collision array
		this->_colArray[0] = this->getPosition();
		for (int i = 4; i > 0; i--)
			this->_colArray[i] = this->_colArray[i - 1];

		// Update internal sprite stuff
		Sprite::Update();
	}
	else {
		this->_dead += elapsedTime;
		if (this->_dead > 4000) {
			this->_x = this->_newSpawn.x;
			this->_y = this->_newSpawn.y;
			this->_lives = 10;
			this->_dead = -1;
		}
	}
}

void Player::draw(Graphics& graphics) {
	Sprite::draw(graphics);
}

void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	//Figure out what side the collision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_y = others.at(i).getBottom() + 1;
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			case sides::NONE:
				break;
			}
		}
	}
}

int Player::getHP() const {
	return this->_lives;
}

bool Player::bulletCollision(const Rectangle &other, const int &id) {
	if (this->_colArray[4].x != 0 && this->getLegacyBoundingBox().collidesWith(other)) {
		if (id != this->_entityID && this->_dead == -1) {
			this->_lives--;
			return true;
		}
	}
	return false;
}

void Player::die(Vector2 & newSpawn) {
	this->_newSpawn = newSpawn;
	this->_dead = 0;
}

int & Player::getState()
{
	return this->_dead;
}

Rectangle Player::getLegacyBoundingBox() const {
	Rectangle box = Rectangle(this->_colArray[4].x, this->_colArray[4].y,
			this->_sourceRect.w, this->_sourceRect.h);
	return box;
}
