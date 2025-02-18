// Chip8Emulator.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

/*Ese programa tiene como fin emular un chip8*/

#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <stdint.h>
#include <SDL.h>
#include <string.h>

#include "machine.h"
#include "screen.h"

const int FPS = 120;
const int frameDelay = 1000 / FPS;


int main(int argc, char* argv[])
{
	Machine tMachine;
	Screen screen;
	SDL_Init(SDL_INIT_EVERYTHING);

	
	initDisplay(&screen);

	void (*opcodesFunc[])(unsigned short*, Machine*) = { opcode0, opcode1, opcode2, opcode3,
													opcode4, opcode5, opcode6, opcode7,
													opcode8, opcode9, opcodeA, opcodeB,
													opcodeC, opcodeD, opcodeE, opcodeF };


	bool run = true;

	unsigned short opcodes[SIZECODES];

	initMachine(&tMachine,opcodes);
	loadRoom(&tMachine);

	while (run)
	{
		Uint32 frameStart = SDL_GetTicks();

		SDL_PollEvent(&screen.event);


		if (screen.event.type == SDL_QUIT) run = false;
		//if (tMachine.pc + 1 >= MEMSIZE) run = false;
		
		//Lee opcode y obtengo informacion para procesar los opcodes
		opcodes[OPCODE] = (tMachine.mem[tMachine.pc] << 8) | (tMachine.mem[tMachine.pc + 1]); 
		tMachine.pc = (tMachine.pc + 2) & 0xFFFF;


		opcodes[IDEN] = (opcodes[OPCODE] & 0xF000) >> 12;
		opcodes[NNN] = (opcodes[OPCODE] & 0x0FFF);
		opcodes[KK] = (opcodes[OPCODE] & 0x00FF) ;
		opcodes[X] = (opcodes[OPCODE] & 0x0F00) >> 8;
		opcodes[Y] = (opcodes[OPCODE] & 0x00F0) >> 4;
		opcodes[R] = (opcodes[OPCODE] & 0x000F);

		//printMem(&tMachine);

		if (tMachine.timers[0]) tMachine.timers[0]--;
		if (tMachine.timers[1]) tMachine.timers[1]--;

		updateScreen(&screen,&tMachine);

		opcodesFunc[opcodes[IDEN]](opcodes, &tMachine);

		int frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	deleteScreen(&screen);
	SDL_Quit();


	return 0;
}


