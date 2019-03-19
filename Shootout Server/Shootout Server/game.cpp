#include <algorithm>
#include "game.h"
#include <iostream>
#include "globals.h"
#include "new_bullet_info.h"

Game::Game() :
	_bulletID(0),
	_new_bullets_len(0)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	this->_level = Level("Level", this->_graphics);
	
	Vector2 mapSize = this->_level.getMapSize();
	std::cout << mapSize.x << " , " << mapSize.y << std::endl;
}

Game::~Game() {

}

bool Game::tick(float elapsedTime) {
	// Start game time
	this->_input.beginNewFrame();
	if (SDL_PollEvent(&this->_event)){
		if (this->_event.type == SDL_QUIT) {
			return false;
		}
	}

	this->update(elapsedTime);
	this->draw(this->_graphics);
	return true;
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	this->_level.draw(graphics);
	for (unsigned int i = 0; i < this->_players.size(); i++) {
		this->_players[i].draw(graphics);
	}
	for (unsigned int i = 0; i < this->_bullets.size(); i++) {
		this->_bullets[i].draw(graphics);
	}

	graphics.flip();
}

void Game::update(float elapsedTime) {
	this->_level.update(elapsedTime);

	// VECTOR FOR COLLISION RECTANGLES
	std::vector<Rectangle> others;
	for (unsigned int i = 0; i < this->_players.size(); i++) {
		// UPDATE
		this->_players[i].update(elapsedTime);

		// COLLISION
		this->_level.checkTileCollisions(this->_players[i].getBoundingBox(), others);
		if (others.size() > 0) {
			this->_players[i].handleTileCollisions(others);
		}
	}

	for (int i = 0; i < this->_bullets.size(); i++) {
		// UPDATE
		this->_bullets[i].update(elapsedTime);

		// COLLISION
		Rectangle bulletBox = this->_bullets.at(i).getBoundingBox();
		if (this->_level.checkTileCollisions(bulletBox, others)) {
			this->delete_bullet(i);
			continue;
		}

		bulletBox = this->_bullets.at(i).getLegacyBoundingBox();
		for (unsigned int k = 0; k < this->_players.size(); k++) {
			if (this->_players[k].bulletCollision(bulletBox, this->_bullets.at(i).getParentID()) == true) {
				this->delete_bullet(i);
				if (this->_players[k].getHP() == 0) {
					this->_players[k].die(this->_level.getNextSpawnpoint());
					
				}

				i -= 1;
				break;
			}
		}
	}
}

void Game::delete_bullet(int index) {
	this->_bullets.erase(this->_bullets.begin() + index);
}

void Game::shoot(Player * player, int x, int y)
{
	if (player->getState() != -1)
		return;

	Rectangle boundingBox = player->getBoundingBox();

	float dx = x - boundingBox.getCenterX();
	float dy = y - boundingBox.getCenterY();

	this->_bullets.push_back(Bullet(this->_graphics, boundingBox, dx, dy, this->_bulletID, player->getID()));

	Bullet *pointer = &this->_bullets.back();
	this->_new_bullets[this->_new_bullets_len].id = pointer->getParentID();
	this->_new_bullets[this->_new_bullets_len].x = pointer->getPosition().x;
	this->_new_bullets[this->_new_bullets_len].y = pointer->getPosition().y;
	this->_new_bullets[this->_new_bullets_len].dx = pointer->getDX() * 10000;
	this->_new_bullets[this->_new_bullets_len].dy = pointer->getDY() * 10000;
	++this->_new_bullets_len;

	++this->_bulletID;
	if (this->_bulletID > 200)
		this->_bulletID = 0;
}

void Game::deletePlayer(Player * player)
{
	int parentID = player->getID();

	for (int i = 0; i < this->_bullets.size(); i++) {
		if (this->_bullets.at(i).getParentID() == parentID) {
			this->delete_bullet(i);
		}
	}

	for (int i = 0; i < this->_players.size(); i++) {
		if (&this->_players.at(i) == player) {
			this->_players.erase(this->_players.begin() + i);
		}
	}
}

Player* Game::newPlayer(int playerId) {
	Vector2 spawnpoint = this->_level.getNextSpawnpoint();
	std::cout << spawnpoint.x << " " << spawnpoint.y << std::endl;

	this->_players.push_back(Player(this->_graphics, spawnpoint.x, spawnpoint.y, playerId));
	Player * playa = &this->_players.back();
	return playa;
}

void Game::generatePacketData(std::stringstream * data_buffer) {
	data_buffer->str("");
	data_buffer->clear();
	*data_buffer << "<p|";

	Vector2 pos;
	for (int p = 0; p < this->_players.size(); p++) {

		pos = this->_players.at(p).getPosition();
		*data_buffer << this->_players.at(p).getID() << ',' << pos.x << ',' << pos.y << ',' << this->_players.at(p).getHP() << ')';
	}

	if (this->_new_bullets_len != 0)
		*data_buffer << '|';
	for (int n = 0; n < this->_new_bullets_len; n++) {
		*data_buffer << this->_new_bullets[n].id << ',' << this->_new_bullets[n].x << ',' << this->_new_bullets[n].y << ',' << this->_new_bullets[n].dx << ',' << this->_new_bullets[n].dy << ')';
	}

	*data_buffer << '>';

	this->_new_bullets_len = 0;
}
