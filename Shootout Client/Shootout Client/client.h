#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "tcp_handler.h"
#include "globals.h"
#include <sstream>
#include "game.h"

struct Click;

class Client {
public:
	Client();
	Client(char* name, char* IP);
	~Client();

private:
	void mainloop();
	void send_data();
	void login(char * name);
	void quit();

	Game _game;
	TCPhandler _tcp_handler;
	std::stringstream _output;
};


#endif
