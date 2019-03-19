#ifndef TCP_HANDLER_H
#define TCP_HANDLER_H

#include <SDL2/SDL_net.h>
#include "client.h"
#include "game.h"
#include <vector>
#include <sstream>

class TCPHandler {
public:
	TCPHandler();
	~TCPHandler();

	void setGame(Game * game) {
		this->game = game;
	}

	void tick();
	void send_data();
	void newPlayer(Player * player, int new_client_id);
	void playerQuit(Player * player, Client* self, TCPsocket* socket);
private:
	Game* game;
	IPaddress _ip;
	SDLNet_SocketSet _sockets;
	TCPsocket _server;
	TCPsocket _tmpsocket;
	int _playerNum, _currentId;
	std::vector<Client> _socketvector;

	std::stringstream _data_buffer;
};

#endif