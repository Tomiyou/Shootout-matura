#ifndef CLIENT_H
#define CLIENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string>
#include <vector>
#include "globals.h"

namespace {
	const int BUFFER_LENGTH = 64;
}

class Player;
class Game;
class TCPHandler;

class Client {
public:
	Client();
	Client(TCPsocket socket, int id, Game* game, TCPHandler* tcp_handler);
	~Client();

	TCPsocket& getSocket();

	void receive();
	void send(std::string* data);

	int getID() const {
		return this->_id;
	}
	void setPlayerPointer(Player * player) {
		this->_player = player;
	}

	Player * getPlayer();

private:
	void read_input(int& i);
	void player_login();
	void player_quit();

	TCPsocket _socket;
	Uint32 _timeout;
	int _id;
	Player* _player;
	Game* _game;
	TCPHandler* _tcp_handler;
	
	char _data_buffer[BUFFER_LENGTH];
};

#endif