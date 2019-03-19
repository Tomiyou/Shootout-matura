#include "player.h"
#include "graphics.h"
#include "globals.h"
#include <iostream>

namespace player_constants {
	const float WALK_SPEED = 0.4f;
	// Location of spriteSheet on disk
	const std::string PLAYER_SS = "content/sprites/player_animation.png";
	const std::string HP_BAR_SS = "content/sprites/player_health_bar.png";
}

Player::Player() {}

Player::Player(Graphics& graphics, float x, float y, int id, char * name) :
	PlayerEntity(graphics, player_constants::PLAYER_SS, 0, 0, 64, 64, 58, 58, x, y, 75, id),
	_facing(RIGHT),
	_movement(player_constants::WALK_SPEED)
{
	for (int i = 0; i < 16; i++)
		this->_name[i] = name[i];

	this->setupAnimations();
	this->playAnimation(0);
}

Player::~Player()
{}

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
	this->playAnimation(0);
}

void Player::update(float &elapsedTime) {
	this->_interpolate.quadratic_player_solve(elapsedTime, this->_x, this->_y);

	/*if (this->_dead == false) {
		this->_x += this->_movement.getdX() * elapsedTime;
		this->_y += this->_movement.getdY() * elapsedTime;
	}*/

	PlayerEntity::update(elapsedTime);
}

void Player::draw(Graphics& graphics) {
	PlayerEntity::draw(graphics);
}

void Player::setupAnimations() {
	this->addAnimation(1, 0, 128, 64, 64);
	this->addAnimation(8, 64, 64, 64, 64);
	this->addAnimation(8, 64, 128, 64, 64);
	this->addAnimation(8, 64, 192, 64, 64);
	this->addAnimation(8, 64, 0, 64, 64);
}

void Player::animationDone(int currentAnimation) {

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

	Sprite::Update();
}

bool Player::bulletCollision(const Rectangle &other) {
	if (this->getBoundingBox().collidesWith(other) && this->_dead == false)
		return true;
	return false;
}

void Player::updatePosition(int ticks, int & x, int & y, int & hp) {
	if (this->_lives != hp) {
		if (hp == 10) {
			this->_interpolate.hardmove(x, y);
			this->_lives = hp;
			this->_dead = false;

			return;
		}

		this->_lives = hp;
		if (this->_lives == 0) {
			++this->_deaths;
			this->_dead = true;
			this->_lastHitID->score();
		}
	}
	this->playAnimation(this->_interpolate.update(ticks, x, y));
	this->_interpolate.prediction_fixMove(this->_x, this->_y);
}

int Player::getHP() {
	return this->_lives;
}
