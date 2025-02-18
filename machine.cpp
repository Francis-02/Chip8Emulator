#include "machine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include<SDL.H>


unsigned char hexcodes[] = {
	// Dígito 0
	0xF0, 0x90, 0x90, 0x90, 0xF0,
	// Dígito 1
	0x20, 0x60, 0x20, 0x20, 0x70,
	// Dígito 2
	0xF0, 0x10, 0xF0, 0x80, 0xF0,
	// Dígito 3
	0xF0, 0x10, 0xF0, 0x10, 0xF0,
	// Dígito 4
	0x90, 0x90, 0xF0, 0x10, 0x10,
	// Dígito 5
	0xF0, 0x80, 0xF0, 0x10, 0xF0,
	// Dígito 6
	0xF0, 0x80, 0xF0, 0x90, 0xF0,
	// Dígito 7
	0xF0, 0x10, 0x20, 0x40, 0x40,
	// Dígito 8
	0xF0, 0x90, 0xF0, 0x90, 0xF0,
	// Dígito 9
	0xF0, 0x90, 0xF0, 0x10, 0xF0,
	// Dígito A
	0xF0, 0x90, 0xF0, 0x90, 0x90,
	// Dígito B
	0xE0, 0x90, 0xE0, 0x90, 0xE0,
	// Dígito C
	0xF0, 0x80, 0x80, 0x80, 0xF0,
	// Dígito D
	0xE0, 0x90, 0x90, 0x90, 0xE0,
	// Dígito E
	0xF0, 0x80, 0xF0, 0x80, 0xF0,
	// Dígito F
	0xF0, 0x80, 0xF0, 0x80, 0x80
};

//Array que recoje los callbacks o punteros a funciones
unsigned char keys[] = {
    SDL_SCANCODE_X,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
    SDL_SCANCODE_V };

int loadRoom(Machine *tMachine)
{
	FILE* rom;
	errno_t err = fopen_s(&rom, "TETRIS.rom", "rb");
	if (err != 0) {
		printf("Rom no encontrado, asegurese de la existencia del archivo");
		return 0;
	}
	fread(&tMachine->mem[ROMINIT], 1, MEMSIZE - ROMINIT, rom);
	fclose(rom);
}
void printMem(Machine *tMachine)
{
    for (int i = 0; i < 32; i++)
    {
        printf("\n");
        for (int j = 0; j < 64; j++)
        {
            if(   ( (tMachine->mem[i * 32 + j] << 8) | (tMachine->mem[(i * 32 + j) + 1] )  ) != 0x0)
            printf("%x ", (tMachine->mem[i*32 + j] << 8) | (tMachine->mem[(i * 32 + j) + 1]));
        }
    }
    system("cls");
}
static int isKeyDown(char key) {
    const Uint8* sdlKeys = SDL_GetKeyboardState(NULL);
    Uint8 realKey = keys[(int)key];
    return sdlKeys[realKey];
}


void initMachine(Machine* tMachine, unsigned short opcodes[]) {



	for (int i = 0; i < SIZECODES; i++)
	{
		tMachine->chip8Display[i] = 0;
	}
	memset(tMachine, 0x00, sizeof(struct Machine));

	tMachine->pc = 0x200;
	memcpy(tMachine->mem + 0x50, hexcodes, 80);


}

void opcode0(unsigned short* opcodes, Machine* tMachine) {
    // 00E0 - CLS
    if (opcodes[NNN] == 0x0E0) {
        memset(tMachine->chip8Display, 0, SIZESCREENCHIP8X * SIZESCREENCHIP8Y);
    }
    // 00EE - RET
    else if (opcodes[NNN] == 0x0EE) {
        if (tMachine->stackP > 0) {
            tMachine->pc = tMachine->stack[--tMachine->stackP];
        }
    }
    else {
        // 0nnn - SYS addr (generalmente ignorado)
    }
}

void opcode1(unsigned short* opcodes, Machine* tMachine) {
    // 1nnn - JP addr
    tMachine->pc = opcodes[NNN];
}

void opcode2(unsigned short* opcodes, Machine* tMachine) {
    // 2nnn - CALL addr
    if (tMachine->stackP < 16) {
        tMachine->stack[tMachine->stackP++] = tMachine->pc;
        tMachine->pc = opcodes[NNN];
    }
}

void opcode3(unsigned short* opcodes, Machine* tMachine) {
    // 3xkk - SE Vx, byte
    if (tMachine->V[opcodes[X]] == opcodes[KK])
        tMachine->pc += 2;
}

void opcode4(unsigned short* opcodes, Machine* tMachine) {
    // 4xkk - SNE Vx, byte
    if (tMachine->V[opcodes[X]] != opcodes[KK])
        tMachine->pc += 2;
}

void opcode5(unsigned short* opcodes, Machine* tMachine) {
    // 5xy0 - SE Vx, Vy
    if (tMachine->V[opcodes[X]] == tMachine->V[opcodes[Y]])
        tMachine->pc += 2;
}

void opcode6(unsigned short* opcodes, Machine* tMachine) {
    // 6xkk - LD Vx, byte
    tMachine->V[opcodes[X]] = opcodes[KK];
}

void opcode7(unsigned short* opcodes, Machine* tMachine) {
    // 7xkk - ADD Vx, byte
    tMachine->V[opcodes[X]] = (tMachine->V[opcodes[X]] + opcodes[KK]) & 0xFF;
}

void opcode8(unsigned short* opcodes, Machine* tMachine) {
    // 8xy0 - 8xyE: operaciones aritméticas y lógicas
    switch (opcodes[R]) {
    case 0x0: // 8xy0 - LD Vx, Vy
        tMachine->V[opcodes[X]] = tMachine->V[opcodes[Y]];
        break;
    case 0x1: // 8xy1 - OR Vx, Vy
        tMachine->V[opcodes[X]] |= tMachine->V[opcodes[Y]];
        break;
    case 0x2: // 8xy2 - AND Vx, Vy
        tMachine->V[opcodes[X]] &= tMachine->V[opcodes[Y]];
        break;
    case 0x3: // 8xy3 - XOR Vx, Vy
        tMachine->V[opcodes[X]] ^= tMachine->V[opcodes[Y]];
        break;
    case 0x4: { // 8xy4 - ADD Vx, Vy (con acarreo)
        int sum = tMachine->V[opcodes[X]] + tMachine->V[opcodes[Y]];
        tMachine->V[0xF] = (sum > 0xFF) ? 1 : 0;
        tMachine->V[opcodes[X]] = sum & 0xFF;
        break;
    }
    case 0x5: { // 8xy5 - SUB Vx, Vy
        tMachine->V[0xF] = (tMachine->V[opcodes[X]] > tMachine->V[opcodes[Y]]) ? 1 : 0;
        tMachine->V[opcodes[X]] = (tMachine->V[opcodes[X]] - tMachine->V[opcodes[Y]]) & 0xFF;
        break;
    }
    case 0x6: {
        // 8xy6 - SHR Vx {, Vy}
        tMachine->V[0xF] = tMachine->V[opcodes[X]] & 0x1;
        tMachine->V[opcodes[X]] >>= 1;
        break;
    }
    case 0x7: {
        // 8xy7 - SUBN Vx, Vy
        tMachine->V[0xF] = (tMachine->V[opcodes[Y]] > tMachine->V[opcodes[X]]) ? 1 : 0;
        tMachine->V[opcodes[X]] = (tMachine->V[opcodes[Y]] - tMachine->V[opcodes[X]]) & 0xFF;
        break;
    }
    case 0xE: {
        // 8xyE - SHL Vx {, Vy}
        tMachine->V[0xF] = (tMachine->V[opcodes[X]] & 0x80) ? 1 : 0;
        tMachine->V[opcodes[X]] = (tMachine->V[opcodes[X]] << 1) & 0xFF;
        break;
    }
    default:
        // Opcode desconocido
        break;
    }
}

void opcode9(unsigned short* opcodes, Machine* tMachine) {
    // 9xy0 - SNE Vx, Vy
    if (tMachine->V[opcodes[X]] != tMachine->V[opcodes[Y]])
        tMachine->pc += 2;
}

void opcodeA(unsigned short* opcodes, Machine* tMachine) {
    // Annn - LD I, addr
    tMachine->I = opcodes[NNN];
}

void opcodeB(unsigned short* opcodes, Machine* tMachine) {
    // Bnnn - JP V0, addr
    tMachine->pc = (tMachine->V[0] + opcodes[NNN]) & 0xFFF;
}

void opcodeC(unsigned short* opcodes, Machine* tMachine) {
    // Cxkk - RND Vx, byte
    tMachine->V[opcodes[X]] = rand() & opcodes[KK];
}

void opcodeD(unsigned short* opcodes, Machine* tMachine) {
    // Dxyn - DRW Vx, Vy, nibble
    tMachine->V[0xF] = 0;
    for (int i = 0; i < opcodes[R]; i++) {
        // Corregido: se usa I + i para cada fila del sprite
        int sprite = tMachine->mem[tMachine->I + i];
        for (int j = 0; j < 8; j++) {
            // Se asume que el sprite se dibuja horizontalmente
            int px = (tMachine->V[opcodes[X]] + j) % SIZESCREENCHIP8X;
            int py = (tMachine->V[opcodes[Y]] + i) % SIZESCREENCHIP8Y;
            int pos = py * SIZESCREENCHIP8X + px;
            int pixel = (sprite & (0x80 >> j)) ? 1 : 0;
            if (pixel) {
                if (tMachine->chip8Display[pos] == 1)
                    tMachine->V[0xF] = 1;
                tMachine->chip8Display[pos] ^= pixel;
            }
        }
    }
}

void opcodeE(unsigned short* opcodes, Machine* tMachine) {
    // Ex9E y ExA1 - SKP y SKNP: Implementación depende de la gestión de entrada
    // Se dejan como stubs para que integres tu manejo del teclado.
    if (opcodes[KK] == 0x9E) {
        if (isKeyDown(tMachine->V[opcodes[X]])) (tMachine->pc = tMachine->pc + 2);
    }
    else if (opcodes[KK] == 0xA1) {
        if (!isKeyDown(tMachine->V[opcodes[X]])) (tMachine->pc = tMachine->pc + 2) ;
    }
}

void opcodeF(unsigned short* opcodes, Machine* tMachine) {
    switch (opcodes[KK]) {
    case 0x07: // Fx07 - LD Vx, DT
        tMachine->V[opcodes[X]] = tMachine->timers[0];
        break;
    case 0x0A: // Fx0A - LD Vx, K
        // Bloquear la ejecución hasta que se presione una tecla.
        // Implementa la lógica según tu sistema de entrada.
        break;
    case 0x15: // Fx15 - LD DT, Vx
        tMachine->timers[0] = tMachine->V[opcodes[X]];
        break;
    case 0x18: // Fx18 - LD ST, Vx
        tMachine->timers[1] = tMachine->V[opcodes[X]];
        break;
    case 0x1E: // Fx1E - ADD I, Vx
        tMachine->I = (tMachine->I + tMachine->V[opcodes[X]]) & 0xFFF;
        break;
    case 0x29: // Fx29 - LD F, Vx
        // Asumiendo que los sprites comienzan en 0x50 y cada uno ocupa 5 bytes.
        tMachine->I = 0x50 + ((tMachine->V[opcodes[X]] & 0xF) * 5);
        break;
    case 0x33: { // Fx33 - LD B, Vx (BCD)
        int value = tMachine->V[opcodes[X]];
        tMachine->mem[tMachine->I] = value / 100;
        tMachine->mem[tMachine->I + 1] = (value / 10) % 10;
        tMachine->mem[tMachine->I + 2] = value % 10;
        break;
    }
    case 0x55: // Fx55 - LD [I], Vx
        for (int i = 0; i <= opcodes[X]; i++) {
            tMachine->mem[tMachine->I + i] = tMachine->V[i];
        }
        break;
    case 0x65: // Fx65 - LD Vx, [I]
        for (int i = 0; i <= opcodes[X]; i++) {
            tMachine->V[i] = tMachine->mem[tMachine->I + i];
        }
        break;
    default:
        // Opcode desconocido
        break;
    }
}