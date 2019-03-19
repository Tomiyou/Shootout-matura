#include <algorithm>
#include "game.h"
#include "graphics.h"
#include "input.h"
#include <iostream>
#include "globals.h"
#include "camera.h"
#include <string>
#include <sstream>

namespace {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}



Game::Game() :
	_graphics(),
	_input(),
	_event(),
	_is_player_init(false),
	_bullet_id(0)
{
	TTF_Init();
}

Game::~Game() {

}

void Game::inputTick()
{
	this->_input.beginNewFrame();
	while (SDL_PollEvent(&this->_event)) {
		if (this->_event.type == SDL_KEYDOWN) {
			if (this->_event.key.repeat == 0) {
				this->_input.keyDownEvent(this->_event);
			}
		}
		else if (this->_event.type == SDL_KEYUP) {
			this->_input.keyUpEvent(this->_event);
		}
		else if (this->_event.type == SDL_MOUSEBUTTONDOWN) {
			this->_input.buttonDownEvent(this->_event);
		}
		else if (this->_event.type == SDL_MOUSEBUTTONDOWN) {
			this->_input.buttonUpEvent(this->_event);
		}
		else if (this->_event.type == SDL_QUIT) {
			std::cout << "PRESS ESCAPE INSTEAD!" << std::endl;
			return;
		}
	}
}

void Game::tick(float elapsedTime) {
	if (this->_is_player_init) {
		this->update(elapsedTime);
		this->draw(this->_graphics);
	}
}

void Game::player_DATA(int & id, int & x, int & y, int & hp)
{
	if (id == this->_player.getID()) {
		this->_player.updatePosition(SDL_GetTicks(), x, y, hp);
	}
	else {
		for (int i = 0; i < this->_enemies.size(); i++) {
			if (id == this->_enemies[i].getID()) {
				this->_enemies.at(i).updatePosition(SDL_GetTicks(), x, y, hp);
			}
		}
	}
}

void Game::bullet(int & id, int & x, int & y, int & dx, int & dy)
{
	this->_bullets.push_back(Bullet(this->_graphics, this->_bullet_id, x, y, dx, dy, id));
	++this->_bullet_id;
	if (this->_bullet_id > 200) {
		this->_bullet_id = 0;
	}
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	this->_level.draw(graphics);
	this->_player.draw(graphics);
	this->_hud.draw(graphics);
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_bullets.size(); i++) {
		this->_bullets.at(i).draw(graphics);
	}


	graphics.flip();
}

void Game::update(float elapsedTime) {
	this->_level.update(elapsedTime);
	this->_player.update(elapsedTime);

	std::vector<Rectangle> others;
	others = this->_level.checkTileCollisions(this->_player.getBoundingBox());
	if (others.size() > 0) {
		this->_player.handleTileCollisions(others);
	}

	this->_camera.update(this->_player.getX(), this->_player.getY());

	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i).update(elapsedTime);
	}

	int id_buf = -1;
	for (int i = 0; i < this->_bullets.size(); i++) {
		id_buf = this->_bullets.at(i).getParentID();

		// UPDATE
		this->_bullets.at(i).update(elapsedTime);

		// COLLISION
		Rectangle bulletBox = this->_bullets.at(i).getBoundingBox();
		if (this->_level.checkTileCollisions(bulletBox).size() > 0) {
			this->delete_bullet(i);
			i -= 1;
			continue;
		}

		if (id_buf != this->_player.getID()) {

			if (this->_player.bulletCollision(bulletBox)) {
				for (int l = 0; l < this->_enemies.size(); l++) {
					if (id_buf == this->_enemies.at(l).getID()) {
						this->_player.setLastHit(this->_enemies.at(l));
					}
				}

				this->delete_bullet(i);
				i -= 1;
				continue;
			}
		}

		for (unsigned int e = 0; e < this->_enemies.size(); e++) {
			if (id_buf == this->_enemies.at(e).getID())
				continue;

			if (this->_enemies.at(e).bulletCollision(bulletBox)) {
				if (id_buf == this->_player.getID()) {
					this->_enemies.at(e).setLastHit(this->_player);
				}
				else {
					for (int l = 0; l < this->_enemies.size(); l++) {
						if (id_buf == this->_enemies.at(l).getID()) {
							this->_enemies.at(e).setLastHit(this->_enemies.at(l));
						}
					}
				}

				this->delete_bullet(i);
				i -= 1;

				break;
			}
		}
	}
}

void Game::delete_bullet(int index)
{
	this->_bullets.erase(this->_bullets.begin() + index);
}

void Game::init_player(int id, int x, int y, char * name)
{
	this->_player = Player(this->_graphics, x, y, id, name);
	this->_level = Level("Level", this->_graphics);
	this->_camera = Camera(this->_player.getSize(), this->_level.getMapSize());
	this->_hud = HUD(this->_graphics, "content/HUD/heart.png", &this->_player);
	this->_graphics.initCamera(&this->_camera);

	Vector2 mapSize = this->_level.getMapSize();

	this->_is_player_init = true;
}

void Game::new_enemy(int x, int y, int id, char* name)
{
	this->_enemies.push_back(Enemy(this->_graphics, x, y, id, name));
}

void Game::del_enemy(int id)
{
	for (int i = 0; i < this->_bullets.size(); i++) {
		if (this->_bullets.at(i).getParentID() == id) {
			this->_bullets.erase(this->_bullets.begin() + i);

			break;
		}
	}

	for (int i = 0; i < this->_enemies.size(); i++) {
		if (this->_enemies.at(i).getID() == id) {
			this->_enemies.erase(this->_enemies.begin() + i);

			break;
		}
	}
}

int Game::input_data(std::stringstream & output) {
	int applied_any_data = 0;

	output.str("");
	output.clear();
	output << "<i";
	// Checking for pressed keys
	if (this->_input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
		return 1;
	}
	if (this->_input.wasKeyPressed(SDL_SCANCODE_LEFT) || this->_input.wasKeyPressed(SDL_SCANCODE_A)) {
		output << "1)";
		applied_any_data = 2;

		this->_player.moveLeft();
	}
	if (this->_input.wasKeyPressed(SDL_SCANCODE_RIGHT) || this->_input.wasKeyPressed(SDL_SCANCODE_D)) {
		output << "3)";
		applied_any_data = 2;
		this->_player.moveRight();
	}
	if (this->_input.wasKeyPressed(SDL_SCANCODE_UP) || this->_input.wasKeyPressed(SDL_SCANCODE_W)) {
		output << "4)";
		applied_any_data = 2;
		this->_player.moveUp();
	}
	if (this->_input.wasKeyPressed(SDL_SCANCODE_DOWN) || this->_input.wasKeyPressed(SDL_SCANCODE_S)) {
		output << "2)";
		applied_any_data = 2;
		this->_player.moveDown();
	}
	if (this->_input.wasKeyPressed(SDL_SCANCODE_TAB)) {
		this->showStats();
	}

	// Checking for released keys
	if (this->_input.wasKeyReleased(SDL_SCANCODE_LEFT) || this->_input.wasKeyReleased(SDL_SCANCODE_A)) {
		output << "5)";
		applied_any_data = 2;
		this->_player.moveRight();
	}
	if (this->_input.wasKeyReleased(SDL_SCANCODE_RIGHT) || this->_input.wasKeyReleased(SDL_SCANCODE_D)) {
		output << "7)";
		applied_any_data = 2;
		this->_player.moveLeft();
	}
	if (this->_input.wasKeyReleased(SDL_SCANCODE_UP) || this->_input.wasKeyReleased(SDL_SCANCODE_W)) {
		output << "8)";
		applied_any_data = 2;
		this->_player.moveDown();
	}
	if (this->_input.wasKeyReleased(SDL_SCANCODE_DOWN) || this->_input.wasKeyReleased(SDL_SCANCODE_S)) {
		output << "6)";
		applied_any_data = 2;
		this->_player.moveUp();
	}

	output << '|';
	Click * click = this->_input.wasButtonPressed(SDL_BUTTON_LEFT);
	if (click != NULL) {
		output << "1,";

		int x = (this->_camera.getX() + click->getX()) / globals::WORLD_SCALE + 0.5f;
		int y = (this->_camera.getY() + click->getY()) / globals::WORLD_SCALE + 0.5f;
		output << x << ',';
		output << y << ')';
		applied_any_data = 2;
	}

	output << '>';

	return applied_any_data;
}

void Game::showStats()
{
	std::cout << "------------------------------------------------" << std::endl;
	std::cout << this->_player.getName() << ' ' << this->_player.getKills() << ' ' << this->_player.getDeaths() << std::endl;

	for (int i = 0; i < this->_enemies.size(); i++) {
		std::cout << this->_enemies.at(i).getName() << ' ' << this->_enemies.at(i).getKills() << ' ' << this->_enemies.at(i).getDeaths() << std::endl;
	}
	std::cout << "------------------------------------------------" << std::endl;
}