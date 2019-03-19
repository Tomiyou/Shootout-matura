#include "enemy.h"
#include <iostream>
#include "globals.h"

namespace enemy_constants {
	const std::string ENEMY_SS = "content/sprites/enemy_animation.png";
	const std::string HP_BAR_SS = "content/sprites/enemy_health_bar.png";
}


Enemy::Enemy()
{}

Enemy::Enemy(Graphics & graphics, float x, float y, int id, char * name) :
	PlayerEntity(graphics, enemy_constants::ENEMY_SS, 0, 0, 64, 64, 58, 58, x, y, 75, id),
	_interpolate(),
	_hp_bar(graphics, enemy_constants::HP_BAR_SS, 60, 14)
{
	for (int i = 0; i < 16; i++)
		this->_name[i] = name[i];
	this->_enemy_name = Text(graphics, name, 58, 29);

	this->setupAnimations();
	this->playAnimation(0);
}

Enemy::~Enemy()
{}

void Enemy::setupAnimations() {
	this->addAnimation(1, 0, 128, 64, 64);
	this->addAnimation(8, 64, 64, 64, 64);
	this->addAnimation(8, 64, 128, 64, 64);
	this->addAnimation(8, 64, 192, 64, 64);
	this->addAnimation(8, 64, 0, 64, 64);
}

void Enemy::animationDone(int currentAnimation) {

}

void Enemy::update(float elapsedTime) {
	this->_interpolate.quadratic_solve(elapsedTime, this->_x, this->_y);
	this->_hp_bar.update(this->_x, this->_y);
	this->_enemy_name.update(this->_x, this->_y);

	PlayerEntity::update(elapsedTime);
}

void Enemy::draw(Graphics& graphics) {
	PlayerEntity::draw(graphics);
	this->_hp_bar.draw(graphics);
	this->_enemy_name.draw(graphics);
}

bool Enemy::bulletCollision(const Rectangle &other) {
	if (this->getBoundingBox().collidesWith(other) && this->_dead == false)
		return true;
	return false;
}

void Enemy::updatePosition(int ticks, int & x, int & y, int & hp) {
	if (this->_lives != hp) {
		if (hp == 10) {
			this->_interpolate.hardmove(x, y);
			this->_lives = hp;
			this->_hp_bar.setHP(hp);
			this->_dead = false;

			return;
		}

		this->_lives = hp;
		this->_hp_bar.setHP(hp);
		if (this->_lives == 0) {
			++this->_deaths;
			this->_dead = true;
			this->_lastHitID->score();
		}
	}
	this->playAnimation(this->_interpolate.update(ticks, x, y));
}
