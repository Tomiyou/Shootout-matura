#include "tcp_handler.h"
#include <iostream>
#include "game.h"
#include "exchange_item.h"
#include <string.h>

TCPhandler::TCPhandler() {}

TCPhandler::TCPhandler(Game * game, char* IP) :
	_ip(),
	_client(),
	_game(game),
	_init_success(false)
{
	SDLNet_Init();
	this->_server = SDLNet_AllocSocketSet(1);

	int kek = SDLNet_ResolveHost(&this->_ip, IP, 7561);
	if (this->_ip.host == INADDR_NONE) {
		return;
	}
	this->_client = SDLNet_TCP_Open(&this->_ip);
	if (this->_client == NULL) {
		std::cout << "ERROR" << std::endl;
	}
	else {
		this->_init_success = true;
		SDLNet_TCP_AddSocket(this->_server, this->_client);
	}
}

TCPhandler::~TCPhandler()
{
	SDLNet_FreeSocketSet(this->_server);
	SDLNet_TCP_Close(this->_client);
	SDLNet_Quit();
}

void TCPhandler::parseLoginData()
{
	while (true) {
		int byte_count = SDLNet_TCP_Recv(this->_client, this->_data_buffer, BUFFER_LENGTH);
		if (byte_count > 0) {
			this->read_data();
			break;
		}
	}
}

void TCPhandler::receive()
{
	if (SDLNet_CheckSockets(this->_server, 0) > 0) {
		int byte_count = SDLNet_TCP_Recv(this->_client, this->_data_buffer, BUFFER_LENGTH);
		if (byte_count > 0) {
			this->read_data();
		}
	}
}

void TCPhandler::send(std::string* data)
{
	SDLNet_TCP_Send(this->_client, data->c_str(), data->length());
}

bool TCPhandler::initSuccess()
{
	return this->_init_success;
}

void TCPhandler::read_data()
{
	int i = 0;
	bool reached_data = false;

	while (true) {
		if (reached_data) {
			this->dataRead_jump(i);

			return;
		}
		else {
			if (this->_data_buffer[i] == '<')
				reached_data = true;
		}
		++i;
	}
}

void TCPhandler::welcome(int position)
{
	bool first = true;
	int i = position;
	int reading_stage = 0;

	int id = 0, x = 0, y = 0;
	char name[16];
	int null_trn = 0;

	while (true) {
		if (this->_data_buffer[i] == ')' || this->_data_buffer[i] == '>') {
			name[null_trn] = '\0';

			if (first) {
				first = false;
				this->_game->init_player(id, x, y, name);
			}
			else {
				this->_game->new_enemy(x, y, id, name);
			}

			id = 0;
			x = 0;
			y = 0;
			reading_stage = 0;
			null_trn = 0;

			if (this->_data_buffer[i] == '>')
				break;
		}
		else if (this->_data_buffer[i] == ',') {
			++reading_stage;
		}
		else {
			if (reading_stage == 0) {
				id *= 10;
				id += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 1) {
				x *= 10;
				x += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 2) {
				y *= 10;
				y += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 3) {
				name[null_trn] = this->_data_buffer[i];
				++null_trn;
			}
		}

		++i;
	}
}

void TCPhandler::new_player(int position)
{
	int i = position;
	int reading_stage = 0;

	int id = 0, x = 0, y = 0;
	char name[16];
	int null_trn = 0;

	while (true) {
		if (this->_data_buffer[i] == '>') {
			name[null_trn] = '\0';
			this->_game->new_enemy(x, y, id, name);


			if (this->_data_buffer[++i] == '<') {
				this->dataRead_jump(++i);
			}
			break;
		}
		else if (this->_data_buffer[i] == ',') {
			++reading_stage;
		}
		else {
			if (reading_stage == 0) {
				id *= 10;
				id += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 1) {
				x *= 10;
				x += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 2) {
				y *= 10;
				y += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 3) {
				name[null_trn] = this->_data_buffer[i];
				++null_trn;
			}
		}

		++i;
	}
}

void TCPhandler::player_quit(int position)
{
	int i = position;
	int id = 0;
	while (true) {
		if (this->_data_buffer[i] == '>') {
			this->_game->del_enemy(id);

			if (this->_data_buffer[++i] == '<') {
				this->dataRead_jump(++i);
			}
			break;
		}
		else {
			id *= 10;
			id += this->_data_buffer[i] - '0';
		}

		++i;
	}
}

void TCPhandler::update(int position)
{
	int i = position;
	int reading_stage = 0;
	int one = 0, two = 0, three = 0, four = 0, five = 0, item_type = -1;
	bool negative = false;

	while (true) {
		if (this->_data_buffer[i] == '>') {
			if (this->_data_buffer[++i] == '<') {
				this->dataRead_jump(++i);
			}
			break;
		}
		else if (this->_data_buffer[i] == ')') {
			if (negative) {
				if (reading_stage == 0)
					one *= -1;
				else if (reading_stage == 1)
					two *= -1;
				else if (reading_stage == 2)
					three *= -1;
				else if (reading_stage == 3)
					four *= -1;
				else
					five *= -1;
			}


			if (item_type == 0) {
				this->_game->player_DATA(one, two, three, four);
			}
			else if (item_type == 1) {
				this->_game->bullet(one, two, three, four, five);
			}
			else {

			}

			one = 0, two = 0, three = 0, four = 0, five = 0, reading_stage = 0;
			negative = false;
		}
		else if (this->_data_buffer[i] == ',') {
			if (negative) {
				if (reading_stage == 0)
					one *= -1;
				else if (reading_stage == 1)
					two *= -1;
				else if (reading_stage == 2)
					three *= -1;
				else if (reading_stage == 3)
					four *= -1;
				else
					five *= -1;
			}

			negative = false;
			++reading_stage;
		}
		else if (this->_data_buffer[i] == '|') {
			++item_type;
		}
		else if (this->_data_buffer[i] == '-') {
			negative = true;
		}
		else {
			if (reading_stage == 0) {
				one *= 10;
				one += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 1) {
				two *= 10;
				two += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 2) {
				three *= 10;
				three += this->_data_buffer[i] - '0';
			}
			else if (reading_stage == 3) {
				four *= 10;
				four += this->_data_buffer[i] - '0';
			}
			else {
				five *= 10;
				five += this->_data_buffer[i] - '0';
			}
		}

		++i;
	}
}

void TCPhandler::dataRead_jump(int i)
{
	if (this->_data_buffer[i] == 'p')
		this->update(i + 1);
	else if (this->_data_buffer[i] == 'w')
		this->welcome(i + 1);
	else if (this->_data_buffer[i] == 'n')
		this->new_player(i + 1);
	else if (this->_data_buffer[i] == 'q')
		this->player_quit(i + 1);
}
