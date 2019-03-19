#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "player.h"
#include "bullet.h"
#include "level.h"
#include "graphics.h"
#include "input.h"
#include <vector>
#include <sstream>
#include "new_bullet_info.h"

class Game {
public:
	Game();
	~Game();

	bool tick(float elapsedTime);

	void generatePacketData(std::stringstream * data_buffer);
	Player* newPlayer(int playerId);
	void shoot(Player* player, int x, int y);
	void deletePlayer(Player* player);

	std::vector<Player>* getPlayerVector() {
		return &this->_players;
	}
private:
	void draw(Graphics &graphics);
	void update(float elapsedTime);
	void delete_bullet(int index);

	Level _level;
	Graphics _graphics;
	Input _input;
	SDL_Event _event;

	std::vector<Player> _players;
	std::vector<Bullet> _bullets;
	int _bulletID;

	New_bullet _new_bullets[32];
	int _new_bullets_len;
};

#endif
