#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include <string>

struct SDL_Renderer;
struct SDL_Window;
class Camera;

class Graphics {
public:
	Graphics();
	~Graphics();

	// Load image (occurs only once) and return it
	SDL_Surface* loadImage(const std::string &filePath);
	// Draw the image to renderer
	void blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
	void blitHUDSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
	void blitTextSurface(SDL_Texture* texture, SDL_Rect* destinationRectangle);
	// Render to screen
	void flip();
	// Clear renderer
	void clear();
	SDL_Renderer* getRenderer() const;

	void initCamera(Camera* camera);
private:
	SDL_Renderer* _renderer;
	SDL_Window* _window;
	std::map<std::string, SDL_Surface*> _spriteSheets;
	Camera* _camera;
};

#endif
