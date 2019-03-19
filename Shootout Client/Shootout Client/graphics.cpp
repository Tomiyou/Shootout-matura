#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "graphics.h"
#include "globals.h"
#include "camera.h"
#include <iostream>

Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Shootout!");
}

Graphics::~Graphics() {
	for (std::map<std::string, SDL_Surface*>::iterator iterator = this->_spriteSheets.begin(); iterator != this->_spriteSheets.end(); iterator++)
		SDL_FreeSurface(iterator->second);

	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
	if (this->_spriteSheets.count(filePath) == 0) {
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return this->_spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	destinationRectangle->x -= this->_camera->getX();
	destinationRectangle->y -= this->_camera->getY();
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::blitHUDSurface(SDL_Texture * texture, SDL_Rect * sourceRectangle, SDL_Rect * destinationRectangle) {
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::blitTextSurface(SDL_Texture* texture, SDL_Rect* destinationRectangle) {
	destinationRectangle->x -= this->_camera->getX();
	destinationRectangle->y -= this->_camera->getY();
	SDL_RenderCopy(this->_renderer, texture, NULL, destinationRectangle);
}

void Graphics::flip() {
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
	return this->_renderer;
}

void Graphics::initCamera(Camera* camera) {
	this->_camera = camera;
}
