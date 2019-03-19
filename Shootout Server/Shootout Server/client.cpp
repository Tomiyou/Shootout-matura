#include "client.h"
#include <iostream>
#include <sstream>
#include "player.h"
#include "game.h"
#include "tcp_handler.h"

Client::Client() {}

Client::Client(TCPsocket socket, int id, Game* game, TCPHandler* tcp_handler) :
	_socket(socket),
	_timeout(0),
	_id(id),
	_player(NULL),
	_game(game),
	_tcp_handler(tcp_handler)
{}

Client::~Client() {}

TCPsocket& Client::getSocket() {
	return this->_socket;
}


void Client::receive() {
	int byte_count = SDLNet_TCP_Recv(this->_socket, this->_data_buffer, BUFFER_LENGTH);

	if (byte_count > 0) {
		if (this->_data_buffer[1] == 'j')
			this->player_login();
		else if (this->_data_buffer[1] == 'i') {
			int i = 0;
			this->read_input(i);
		}
		else if (this->_data_buffer[1] == 'q') {
			this->player_quit();
		}
	}
}

void Client::send(std::string* data) {
	SDLNet_TCP_Send(this->_socket, data->c_str(), 200);
}



Player * Client::getPlayer() {
	return this->_player;
}

void Client::read_input(int& i) {
	this->_data_buffer[i] = -52;
	this->_data_buffer[++i] = -52;

	int one = 0, two = 0, three = 0, reading_stage = 0, data_type = 0;

	while (true) {
		if (this->_data_buffer[++i] == '>') {
			this->_data_buffer[i] = -52;

			if (this->_data_buffer[++i] == '<') {
				this->read_input(i);
			}

			return;
		}
		else if (this->_data_buffer[i] == '|') {
			++data_type;
		}
		else if (this->_data_buffer[i] == ')') {
			if (data_type == 0) {

				if (one == 1)
					this->_player->moveLeft();
				else if (one == 3)
					this->_player->moveRight();
				else if (one == 4)
					this->_player->moveUp();
				else if (one == 2)
					this->_player->moveDown();
				else if (one == 5)
					this->_player->moveRight();
				else if (one == 7)
					this->_player->moveLeft();
				else if (one == 8)
					this->_player->moveDown();
				else
					this->_player->moveUp();
			}
			else {
				this->_game->shoot(this->_player, two, three);
			}

			one = 0;
			two = 0;
			three = 0;
			reading_stage = 0;
		}
		else if (this->_data_buffer[i] == ',')
			++reading_stage;
		else {
			if (reading_stage == 0) {
				one *= 10;
				one += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 1) {
				two *= 10;
				two += this->_data_buffer[i] - '0';
			}
			else {
				three *= 10;
				three += this->_data_buffer[i] - '0';
			}
		}

		this->_data_buffer[i] = -52;
	}
}

void Client::player_login()
{
	this->_player = this->_game->newPlayer(this->_id);

	char* name = this->_player->getName();
	
	int i = 1;
	while (true) {
		i++;

		if (this->_data_buffer[i] == '>')
			break;

		name[i - 2] = this->_data_buffer[i];
	}
	name[i - 2] = '\0';

	std::cout << this->_player->getName() << std::endl;

	this->_tcp_handler->newPlayer(this->_player, this->_id);
}

void Client::player_quit() {
	this->_tcp_handler->playerQuit(this->_player, this, &this->_socket);

	SDLNet_TCP_Close(this->_socket);
}