#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "level.h"
#include "camera.h"
#include "hud.h"
#include "graphics.h"
#include "input.h"
#include <vector>

struct Click;
struct String;

class Game {
public:
	Game();
	~Game();

	void inputTick();
	void tick(float elapsedTime);
	void player_DATA(int &id, int &x, int &y, int &hp);
	void bullet(int &id, int &x, int &y, int &dx, int &dy);

	void init_player(int id, int x, int y, char * name);
	void new_enemy(int x, int y, int id, char* name);
	void del_enemy(int id);

	int input_data(std::stringstream & output);
private:
	void draw(Graphics &graphics);
	void update(float elapsedTime);
	void delete_bullet(int index);
	bool _is_player_init;
	int _bullet_id;
	void showStats();

	Player _player;
	std::vector<Enemy> _enemies;
	std::vector<Bullet> _bullets;

	Level _level;
	Camera _camera;
	HUD _hud;
	Graphics _graphics;
	Input _input;
	SDL_Event _event;
};

#endif
