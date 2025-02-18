#ifndef SCREEN_H
#define SCREEN_H


#include "machine.h"
#include <SDL.h>

struct Screen {

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	SDL_Texture* texture;

	int pixelBuffer[SIZESCREENX * SIZESCREENY];
};
int initDisplay(Screen* screen);
void updateScreen(Screen* screen, Machine* tMachine);
void deleteScreen(Screen* screen);
void adminKeyBoard(Screen* screen, Machine* tMachine);

#endif //SCREEN_H
