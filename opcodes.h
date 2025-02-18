#ifndef OPCODES_H
#define OPCODES_H

unsigned char hexcodes[] = {
	// D�gito 0
	0xF0, 0x90, 0x90, 0x90, 0xF0,
	// D�gito 1
	0x20, 0x60, 0x20, 0x20, 0x70,
	// D�gito 2
	0xF0, 0x10, 0xF0, 0x80, 0xF0,
	// D�gito 3
	0xF0, 0x10, 0xF0, 0x10, 0xF0,
	// D�gito 4
	0x90, 0x90, 0xF0, 0x10, 0x10,
	// D�gito 5
	0xF0, 0x80, 0xF0, 0x10, 0xF0,
	// D�gito 6
	0xF0, 0x80, 0xF0, 0x90, 0xF0,
	// D�gito 7
	0xF0, 0x10, 0x20, 0x40, 0x40,
	// D�gito 8
	0xF0, 0x90, 0xF0, 0x90, 0xF0,
	// D�gito 9
	0xF0, 0x90, 0xF0, 0x10, 0xF0,
	// D�gito A
	0xF0, 0x90, 0xF0, 0x90, 0x90,
	// D�gito B
	0xE0, 0x90, 0xE0, 0x90, 0xE0,
	// D�gito C
	0xF0, 0x80, 0x80, 0x80, 0xF0,
	// D�gito D
	0xE0, 0x90, 0x90, 0x90, 0xE0,
	// D�gito E
	0xF0, 0x80, 0xF0, 0x80, 0xF0,
	// D�gito F
	0xF0, 0x80, 0xF0, 0x80, 0x80
};


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



struct Machine {

	unsigned char mem[MEMSIZE];

	unsigned short pc;

	unsigned short I;

	unsigned char V[SIZEREGV];

	unsigned short stack[SIZESTACK];
	unsigned char stackP;

	unsigned char timers[SIZETIMERS];

	unsigned char chip8Display[SIZESCREENCHIP8X * SIZESCREENCHIP8Y];

};

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


#endif // MI_CABECERA_H
