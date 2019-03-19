#ifndef ENEMY_H
#define EMEMY_H

#include "animatedsprite.h"
#include "globals.h"
#include "Interpolator.h"
#include "hp_bar.h"
#include "text.h"

class Graphics;

class Enemy : public PlayerEntity {
public:
	Enemy();
	Enemy(Graphics &graphics, float x, float y, int id, char * name);
	~Enemy();

	void draw(Graphics &graphics);
	void update(float elapsedTime);
	bool bulletCollision(const Rectangle &other);

	virtual void animationDone(int currentAnimation);
	void setupAnimations();
	void updatePosition(int ticks, int &x, int &y, int &hp);
private:
	Interpolate _interpolate;
	HP_bar _hp_bar;
	Text _enemy_name;
};

#endif