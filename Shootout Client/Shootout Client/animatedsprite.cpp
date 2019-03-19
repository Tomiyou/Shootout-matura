#include "animatedsprite.h"
#include "graphics.h"
#include "sprite.h"
#include <iostream>

PlayerEntity::PlayerEntity() {}

PlayerEntity::PlayerEntity(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int sourceW, int sourceH, int width, int height, float posX, float posY, float timeToUpdate, int id) :
	Sprite(graphics, filePath, sourceX, sourceY, sourceW, sourceH, width, height, posX, posY),
	_frameIndex(0),
	_timeToUpdate(timeToUpdate),
	_visible(true),
	_currentAnimationOnce(false),
	_currentAnimation(0),
	_timeElapsed(0),
	_animationCount(0),
	_lives(10),
	_id(id),
	_dead(false),
	_kills(0),
	_deaths(0),
	_lastHitID(NULL)
{}

PlayerEntity::~PlayerEntity()
{}

void PlayerEntity::addAnimation(int frames, int x, int y, int width, int height) {
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { x + i * width, y, width, height };
		rectangles.push_back(newRect);
	}
	this->_animations[this->_animationCount] = rectangles;
	++this->_animationCount;
}

void PlayerEntity::resetAnimations() {
	//this->_animations.clear();
}

void PlayerEntity::stopAnimation() {
	this->_frameIndex = 0;
	this->animationDone(0);
}

void PlayerEntity::playAnimation(int animation, bool once) {
	this->_currentAnimationOnce = once;
	if (this->_currentAnimation != animation) {
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
	}
}

void PlayerEntity::setVisible(bool visible) {
	this->_visible = visible;
}

void PlayerEntity::setLastHit(PlayerEntity & id)
{
	this->_lastHitID = &id;
}

void PlayerEntity::update(float &elapsedTime) {
	Sprite::Update();

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) {
		this->_timeElapsed -= this->_timeToUpdate;

		if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1) {
			this->_frameIndex++;
		}
		else {
			if (this->_currentAnimationOnce == true) {
				this->setVisible(false);
				this->animationDone(this->_currentAnimation);
			}
			this->_frameIndex = 0;
		}
	}
}

void PlayerEntity::draw(Graphics & graphics) {
	if (this->_visible == true) {
		graphics.blitSurface(this->_spriteSheet, &this->_animations[this->_currentAnimation][this->_frameIndex], &this->_destinationRect);
	}
}

void PlayerEntity::score()
{
	++this->_kills;
}
