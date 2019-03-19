#include <SDL2/SDL.h>
#include "client.h"
#include "game.h"
#include <iostream>
#include "click.h"

Client::Client() {};

Client::Client(char * name, char* IP) :
	_game(),
	_tcp_handler(&_game, IP)
{
	if (this->_tcp_handler.initSuccess() == false)
		return;

	this->login(name);
	this->_tcp_handler.parseLoginData();

	this->mainloop();
}

Client::~Client() {
	SDL_Quit();
}

void Client::mainloop() {
	// MAIN LOOP
	bool running = true;
	int LAST_UPDATE_TIME = SDL_GetTicks();
	while (running) {

		const int CURRENT_TIME_MS = SDL_GetTicks();
		const int ELAPSED_UPDATE_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		// INPUT HANDLING
		this->_game.inputTick();

		int data = this->_game.input_data(this->_output);
		if (data == 1) {
			running = false;
			this->quit();
		}
		else if (data == 2)
			this->send_data();

		// GAME TICK - 16
		if (ELAPSED_UPDATE_TIME_MS >= 16) {
			this->_game.tick(ELAPSED_UPDATE_TIME_MS);
			LAST_UPDATE_TIME = CURRENT_TIME_MS;
		}

		// TCP SOCKET TICK
		this->_tcp_handler.receive();
	}
}

void Client::send_data() {
	this->_tcp_handler.send(&this->_output.str());
}

void Client::login(char * name)
{
	this->_output.str("");
	this->_output.clear();
	this->_output << "<j" << name << '>';
	this->_tcp_handler.send(&this->_output.str());
}

void Client::quit()
{
	this->_output.str("");
	this->_output.clear();
	this->_output << "<q>";
	this->_tcp_handler.send(&this->_output.str());
}
