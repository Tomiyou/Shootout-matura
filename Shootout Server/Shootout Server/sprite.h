#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <string>

#include "globals.h"
#include "rectangle.h"

class Graphics;

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY, int id);
	virtual ~Sprite();
	virtual void Update();
	void draw(Graphics &graphics);

	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle &other) const;

	Vector2 getSize() const;
	Vector2 getPosition() const;

	const int getID() const;

protected:
	int _entityID;

	SDL_Rect _sourceRect, _destinationRect;
	SDL_Texture* _spriteSheet;
	float _x, _y;

	Rectangle _boundingBox;

private:

};

#endif
