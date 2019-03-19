#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include <map>
#include <string>
#include <vector>

#include "sprite.h"
#include "globals.h"

class Graphics;

class PlayerEntity : public Sprite {
public:
	PlayerEntity();
	PlayerEntity(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int sourceW, int sourceH, int width, int height, float posX, float posY, float timeToUpdate, int id);
	~PlayerEntity();

	void playAnimation(int animation, bool once = false);
	void update(float &elapsedTime);
	void draw(Graphics &graphics);

	void score();
	void setLastHit(PlayerEntity& id);

	int getID() const {
		return _id;
	}
	int getKills() const {
		return _kills;
	}
	int getDeaths() const {
		return _deaths;
	}
	char* getName() {
		return this->_name;
	}
	bool getState() const {
		return this->_dead;
	}
protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	int _currentAnimation;

	void addAnimation(int frames, int x, int y, int width, int height);
	void resetAnimations();
	void stopAnimation();
	void setVisible(bool visible);
	virtual void animationDone(int currentAnimation) = 0;
	virtual void setupAnimations() = 0;

	int _id, _lives, _kills, _deaths;
	PlayerEntity* _lastHitID;
	char _name[16];
	bool _dead;

private:
	std::vector<SDL_Rect> _animations[5];

	int _frameIndex, _animationCount;
	double _timeElapsed;
	bool _visible;

};

#endif
