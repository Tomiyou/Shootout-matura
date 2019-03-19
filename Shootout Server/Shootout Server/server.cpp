#include <SDL2/SDL.h>
#include "server.h"
#include "game.h"
#include <sstream>

namespace {
	// Tick delay = 10 ms
	const int TICKS_PER_SECOND = 100;
	const int TICK_MS = 1000 / TICKS_PER_SECOND;
	// Packet delay = 20 ms
	const int PACKETS_PER_SECOND = 25;
	const int PACKET_MS = 1000 / PACKETS_PER_SECOND;
}

Server::Server()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	this->Serverloop();
}

Server::~Server() {
	SDL_Quit();
}

void Server::Serverloop() {
	Game game;
	this->_tcp_handler.setGame(&game);


	int LAST_UPDATE_TIME = SDL_GetTicks();
	int LAST_COMM_TIME = SDL_GetTicks();
	while(1) {
		const int CURRENT_TIME_MS = SDL_GetTicks();

		const int ELAPSED_UPDATE_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		if (ELAPSED_UPDATE_TIME_MS >= TICK_MS) {
			if (game.tick(ELAPSED_UPDATE_TIME_MS) == false)
				return;
			LAST_UPDATE_TIME = CURRENT_TIME_MS;
		}

		const int ELAPSED_COMM_TIME_MS = CURRENT_TIME_MS - LAST_COMM_TIME;
		if (ELAPSED_COMM_TIME_MS >= PACKET_MS) {
			this->_tcp_handler.send_data();
			LAST_COMM_TIME = CURRENT_TIME_MS;
		}

		this->_tcp_handler.tick();
	}
}
