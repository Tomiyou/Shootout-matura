#include "client.h"
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	std::cout << "Vnesite ime: ";
	char name[16];
	std::cin >> name;

	std::cout << "Vnesite IP naslov: ";
	char ip[16];
	std::cin >> ip;

	Client client(name, ip);
	return 0;
}
