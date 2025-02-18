#ifndef OPCODES_H
#define OPCODES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define SIZESCREENX 640
#define SIZESCREENY 320

#define SIZESCREENCHIP8X 64
#define SIZESCREENCHIP8Y 32

#define MEMSIZE 4096	
#define ROMINIT 0x200

#define SIZEREGV 16
#define SIZESTACK 16
#define SIZETIMERS 2
#define SIZECODES 7

#define OPCODE 0
#define IDEN 1
#define NNN 2
#define KK 3
#define X 4
#define Y 5
#define R 6




unsigned char hexcodes[];
unsigned char keys[];



struct Machine {

	unsigned char mem[MEMSIZE];

	unsigned short pc;

	unsigned short I;

	unsigned char V[SIZEREGV];

	unsigned short stack[SIZESTACK];
	unsigned char stackP;

	unsigned char timers[SIZETIMERS];

	unsigned char chip8Display[SIZESCREENCHIP8X * SIZESCREENCHIP8Y];
	unsigned char keyBoard;
	bool keyState;

};

void printMem(Machine* tMachine);
int loadRoom(Machine* tMachine);
void printMemMachine(Machine* tMachine);

void initMachine(Machine* tMachine, unsigned short opcodes[]);

static int isKeyDown(char key);

void opcode0(unsigned short* opcodes, Machine* tMachine);
void opcode1(unsigned short* opcodes, Machine* tMachine);
void opcode2(unsigned short* opcodes, Machine* tMachine);
void opcode3(unsigned short* opcodes, Machine* tMachine);
void opcode4(unsigned short* opcodes, Machine* tMachine);
void opcode5(unsigned short* opcodes, Machine* tMachine);
void opcode6(unsigned short* opcodes, Machine* tMachine);
void opcode7(unsigned short* opcodes, Machine* tMachine);
void opcode8(unsigned short* opcodes, Machine* tMachine);
void opcode9(unsigned short* opcodes, Machine* tMachine);
void opcodeA(unsigned short* opcodes, Machine* tMachine);
void opcodeB(unsigned short* opcodes, Machine* tMachine);
void opcodeC(unsigned short* opcodes, Machine* tMachine);
void opcodeD(unsigned short* opcodes, Machine* tMachine);
void opcodeE(unsigned short* opcodes, Machine* tMachine);
void opcodeF(unsigned short* opcodes, Machine* tMachine);


#endif //OPCODES_H
