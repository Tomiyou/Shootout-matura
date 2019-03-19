#include "tcp_handler.h"
#include <iostream>
#include "globals.h"

namespace {
	const int MAX_PLAYERS = 16;
}

TCPHandler::TCPHandler() :
	game(game),
	_playerNum(0),
	_currentId(0)
{
	SDLNet_Init();

	SDLNet_ResolveHost(&this->_ip, NULL, 7561);
	this->_sockets = SDLNet_AllocSocketSet(MAX_PLAYERS);
	this->_server = SDLNet_TCP_Open(&this->_ip);
}

TCPHandler::~TCPHandler()
{
	SDLNet_FreeSocketSet(this->_sockets);
	SDLNet_TCP_Close(this->_server);
	SDLNet_Quit();
}

void TCPHandler::tick()
{
	this->_tmpsocket = SDLNet_TCP_Accept(this->_server);

	if (this->_tmpsocket != NULL) {
		if (this->_playerNum < MAX_PLAYERS) {
			SDLNet_TCP_AddSocket(this->_sockets, this->_tmpsocket);
			std::cout << "New peer!" << std::endl;
			this->_socketvector.push_back(Client(this->_tmpsocket, this->_currentId, this->game, this));
			
			++this->_playerNum;
			++this->_currentId;
		}
		else {
			std::cout << "Max clients!" << std::endl;
		}
	}
	this->_tmpsocket = NULL;

	if (SDLNet_CheckSockets(this->_sockets, 0) > 0) {
		for (unsigned int i = 0; i < this->_socketvector.size(); i++) {
			if (SDLNet_SocketReady(this->_socketvector.at(i).getSocket())) {
				this->_socketvector.at(i).receive();
			}
		}
	}
}

void TCPHandler::send_data()
{
	if (this->_socketvector.size() > 0) {
		std::stringstream * pointer = &this->_data_buffer;
		this->game->generatePacketData(pointer);
		for (unsigned int i = 0; i < this->_socketvector.size(); i++) {
			this->_socketvector.at(i).send(&this->_data_buffer.str());
		}
	}
}

void TCPHandler::newPlayer(Player * player, int new_client_id)
{
	std::vector<Player>* player_vector = this->game->getPlayerVector();

	// TELL EVERYONE OF THE NEW PLAYER
	this->_data_buffer.str("");
	this->_data_buffer.clear();
	this->_data_buffer << "<n";

	Vector2 position = player->getPosition();
	this->_data_buffer << player->getID() << ',' << position.x << ',' << position.y << ',' << player->getName() << '>';

	int new_client_index = 0;
	for (int i = 0; i < this->_socketvector.size(); i++) {
		if (this->_socketvector.at(i).getID() == new_client_id)
			new_client_index = i;
		else {
			this->_socketvector.at(i).send(&this->_data_buffer.str());

			for (int k = 0; k < player_vector->size(); k++) {
				if (player_vector->at(k).getID() == this->_socketvector.at(i).getID()) {
					this->_socketvector.at(i).setPlayerPointer(player_vector->at(k).getPointer());
					break;
				}
			}
		}
	}

	this->_data_buffer.str("");
	this->_data_buffer.clear();
	this->_data_buffer << "<w";
	this->_data_buffer << player->getID() << ',' << position.x << ',' << position.y << ',' << player->getName();

	for (int i = 0; i < this->_socketvector.size(); i++) {
		if (this->_socketvector.at(i).getID() == new_client_id)
			continue;
		Player * playa = this->_socketvector.at(i).getPlayer();
		position = playa->getPosition();
		this->_data_buffer << ')' << this->_socketvector.at(i).getID() << ',' << position.x << ',' << position.y << ',' << playa->getName();
	}
	this->_data_buffer << ">";

	this->_socketvector.at(new_client_index).send(&this->_data_buffer.str());
}

void TCPHandler::playerQuit(Player * player, Client* self, TCPsocket* socket)
{
	SDLNet_TCP_DelSocket(this->_sockets, *socket);

	this->_data_buffer.str("");
	this->_data_buffer.clear();
	this->_data_buffer << "<q" << player->getID() << '>';
	for (int i = 0; i < this->_socketvector.size(); i++) {
		if (&this->_socketvector.at(i) == self) {
			this->_socketvector.erase(this->_socketvector.begin() + i);
		}
		else {
			this->_socketvector.at(i).send(&this->_data_buffer.str());
		}
	}

	this->game->deletePlayer(player);
}
