#ifndef TCP_HANDLER_H
#define TCP_HANDLER_H

#include <SDL2/SDL_net.h>
#include <string>
#include <vector>

namespace {
	const int BUFFER_LENGTH = 4096;
}

class Game;

class TCPhandler {
public:
	TCPhandler();
	TCPhandler(Game * game, char* IP);
	~TCPhandler();

	void receive();
	void parseLoginData();
	void send(std::string* data);

	bool initSuccess();
private:
	void read_data();

	void welcome(int position);
	void new_player(int position);
	void player_quit(int position);
	void update(int position);
	void dataRead_jump(int i);

	IPaddress _ip;
	TCPsocket _client;
	SDLNet_SocketSet _server;
	Game* _game;

	char _data_buffer[BUFFER_LENGTH];
	bool _init_success;
};

#endif