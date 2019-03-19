#include <string>
#include "globals.h"
#include "tcp_handler.h"

class Game;

class Server {
public:
	Server();
	~Server();
private:
	void Serverloop();

	TCPHandler _tcp_handler;
};
