#ifndef HP_BAR
#define HP_BAR

#include <string>
#include <vector>
#include <SDL2\SDL.h>

class Graphics;

class HP_bar {
public:
	HP_bar();
	HP_bar(Graphics &graphics, const std::string &filePath, int width, int height);
	~HP_bar();

	void setHP(int hp);
	void update(float &x, float &y);
	void draw(Graphics &graphics);
private:
	int _current_frame, _offset;
	std::vector<SDL_Rect> _frames;
	SDL_Rect _destination_rectangle;
	SDL_Texture * _spriteSheet;
};

#endif