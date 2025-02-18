#include <SDL.h>
#include "screen.h"

int initDisplay(Screen* screen)
{
	screen->window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SIZESCREENX, SIZESCREENY, SDL_WINDOW_SHOWN);
	screen->renderer = SDL_CreateRenderer(screen->window, -1, SDL_RENDERER_ACCELERATED);
	screen->event;
	screen->texture = SDL_CreateTexture(
		screen->renderer, SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		SIZESCREENCHIP8X,
		SIZESCREENCHIP8Y);

	if (!screen->window)
	{
		printf("Error al crear la ventana");
		return 1;
	}
	if (!screen->renderer)
	{
		printf("Error al crear el render");
		return 1;
	}
	if (!screen->texture)
	{
		printf("Error al crear la textura");
		return 1;
	}
}
void updateScreen(Screen* screen, Machine* tMachine)
{

	// Convertir el array de display a un buffer de pixeles
	// Si chip8Display[i] es 1, se asigna color blanco; si es 0, color negro.
	for (int y = 0; y < SIZESCREENCHIP8X; y++) {
		for (int x = 0; x < SIZESCREENCHIP8Y; x++) {
			int index = y * SIZESCREENCHIP8Y + x;
			if (tMachine->chip8Display[index] == 1)
				screen->pixelBuffer[index] = 0xFF000000;  // Blanco: (alpha 255, red 255, green 255, blue 255)
			else
				screen->pixelBuffer[index] = 0xFF0CFFC7;  // Negro: (alpha 255, red 0, green 0, blue 0)
		}
	}

	SDL_UpdateTexture(screen->texture, NULL, screen->pixelBuffer, SIZESCREENCHIP8X * sizeof(int));
	SDL_RenderClear(screen->renderer);
	SDL_RenderCopy(screen->renderer, screen->texture, NULL, NULL);
	SDL_RenderPresent(screen->renderer);

}
void deleteScreen(Screen* screen)
{
	SDL_DestroyTexture(screen->texture);
	SDL_DestroyRenderer(screen->renderer);
	SDL_DestroyWindow(screen->window);
}



