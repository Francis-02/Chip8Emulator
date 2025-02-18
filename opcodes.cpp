#include "opcodes.h"

void opcode0(unsigned short* opcodes, Machine* tMachine) {
	if (opcodes[NNN] == 0x0e0) {
		printf("cls\n");
		memset(tMachine->chip8Display, 0, 2048);
	}
	else if (opcodes[NNN] == 0x0ee) {
		printf("RET\n");
		if (tMachine->stackP > 0)
		{
			tMachine->pc = tMachine->stack[--tMachine->stackP];
		}

	}
	else {
		printf("eerror");
	}
}
void opcode1(unsigned short* opcodes, Machine* tMachine) {
	printf("JP %x\n", opcodes[NNN]);
	tMachine->pc = opcodes[NNN];
}
void opcode2(unsigned short* opcodes, Machine* tMachine) {
	printf("Call % x\n", opcodes[NNN]);
	if (tMachine->stackP < 16)
	{
		tMachine->stack[tMachine->stackP++] = tMachine->pc;
		tMachine->pc = opcodes[NNN];
	}
}
void opcode3(unsigned short* opcodes, Machine* tMachine) {
	printf("SE %x ,%x \n", opcodes[X], opcodes[KK]);
	if (tMachine->V[opcodes[X]] == opcodes[KK])
	{
		tMachine->pc = (tMachine->pc + 2) & 0xFFF;
	}
}
void opcode4(unsigned short* opcodes, Machine* tMachine) {
	printf("SNE %x ,%x \n", opcodes[X], opcodes[KK]);
	if (tMachine->V[opcodes[X]] != opcodes[KK])
	{
		tMachine->pc = (tMachine->pc + 2) & 0xFFF;
	}
}
void opcode5(unsigned short* opcodes, Machine* tMachine) {
	printf("SE %x ,%x \n", opcodes[X], opcodes[Y]);
	if (tMachine->V[opcodes[X]] == tMachine->V[opcodes[Y]])
	{
		tMachine->pc = (tMachine->pc + 2) & 0xFFF;
	}
}
void opcode6(unsigned short* opcodes, Machine* tMachine) {
	printf("LD %x , %x \n", opcodes[X], opcodes[KK]);
	tMachine->V[opcodes[X]] = opcodes[KK];
}
void opcode7(unsigned short* opcodes, Machine* tMachine) {
	printf("ADD %x , %x \n", opcodes[X], opcodes[KK]);
	tMachine->V[opcodes[X]] += opcodes[KK];
}
void opcode8(unsigned short* opcodes, Machine* tMachine) {
	switch (opcodes[R])
	{
	case 0:
		printf("LD %x, %x \n", opcodes[X], opcodes[Y]);
		tMachine->V[opcodes[X]] = tMachine->V[opcodes[Y]];
		break;
	case 1:
		printf("OR %x, %x \n", opcodes[X], opcodes[Y]);
		tMachine->V[opcodes[X]] |= tMachine->V[opcodes[Y]];
		break;
	case 2:
		printf("AND %x, %x \n", opcodes[X], opcodes[Y]);
		tMachine->V[opcodes[X]] &= tMachine->V[opcodes[Y]];
		break;
	case 3:
		printf("XOR %x, %x \n", opcodes[X], opcodes[Y]);
		tMachine->V[opcodes[X]] ^= tMachine->V[opcodes[Y]];
		break;
	case 4:  // ADD Vx, Vy
		tMachine->V[0xF] = (tMachine->V[opcodes[X]] > (tMachine->V[opcodes[Y]] & 0xFF));
		tMachine->V[opcodes[X]] += tMachine->V[opcodes[Y]];
		break;
	case 5:  // SUB Vx, Vy
		tMachine->V[0xF] = (tMachine->V[opcodes[X]] > tMachine->V[opcodes[Y]]);
		tMachine->V[opcodes[X]] -= tMachine->V[opcodes[Y]];
		break;
	case 6:
		printf("SHR %x, %x \n", opcodes[X]);
		tMachine->V[0xF] = tMachine->V[opcodes[X]] & 0x1;
		tMachine->V[opcodes[X]] >>= 1;
		break;
	case 7:
		printf("SUBN %x, %x \n", opcodes[X], opcodes[Y]);
		tMachine->V[0xF] = (tMachine->V[opcodes[Y]] > tMachine->V[opcodes[X]]);
		tMachine->V[opcodes[X]] = (tMachine->V[opcodes[Y]] - tMachine->V[opcodes[X]]);
		break;
	case 0xE:  // SHL Vx
		tMachine->V[0xF] = ((tMachine->V[opcodes[X]] & 0x80) != 0);  // Bit más significativo
		tMachine->V[opcodes[X]] <<= 1;
		break;
	}
}
void opcode9(unsigned short* opcodes, Machine* tMachine) {
	printf("SNE %x, %x \n", opcodes[X], opcodes[Y]);
	if (tMachine->V[opcodes[X]] != tMachine->V[opcodes[Y]])
	{
		tMachine->pc = (tMachine->pc + 2) & 0xFFF;
	}
}
void opcodeA(unsigned short* opcodes, Machine* tMachine) {
	printf("LD, I %x \n", opcodes[NNN]);
	tMachine->I = opcodes[NNN];
}
void opcodeB(unsigned short* opcodes, Machine* tMachine) {
	printf("JP V0 %x \n", opcodes[NNN]);
	tMachine->pc = (tMachine->V[0] + opcodes[NNN]) & 0xFFF;
}
void opcodeC(unsigned short* opcodes, Machine* tMachine) {
	printf("RND V0 %x, %x \n", opcodes[X], opcodes[KK]);
	tMachine->V[opcodes[X]] = rand() & opcodes[KK];
}
void opcodeD(unsigned short* opcodes, Machine* tMachine) {
	printf("DWR  %x, %x, %x \n", opcodes[X], opcodes[Y], opcodes[R]);

	tMachine->V[15] = 0;
	for (int i = 0; i < opcodes[R]; i++)
	{
		int sprite = tMachine->mem[tMachine->I];
		for (int j = 0; j < 8; j++) {
			int px = (tMachine->V[opcodes[X]] + i) & 63;
			int py = (tMachine->V[opcodes[Y]] + j) & 31;
			int pos = 64 * py + px;
			int pixel = (sprite & (1 << (7 - j))) != 0;

			tMachine->V[15] != (tMachine->chip8Display[pos] & pixel);
			tMachine->chip8Display[pos] ^= pixel;
			//tMachine->chip8Display[SIZESCREENCHIP8X * py + px] ^= (sprite & (1 << (7 - i))) != 0;
		}
	}

}
void opcodeE(unsigned short* opcodes, Machine* tMachine) {
	if (opcodes[KK] == 0x9e) {
		printf("SKP %x \n", opcodes[X]);
	}
	else if (opcodes[KK] == 0xa1)
	{
		printf("SKNP %x \n", opcodes[X]);
	}
}
void opcodeF(unsigned short* opcodes, Machine* tMachine) {
	switch (opcodes[KK])
	{
	case 0x07:
		printf("LD %x, DT \n", opcodes[X]);
		tMachine->V[opcodes[X]] = tMachine->timers[0];
		break;
	case 0x0A:
		printf("LD %x, K \n", opcodes[X]);
		break;
	case 0x15:
		printf("LD %x, K \n", opcodes[X]);
		tMachine->timers[0] = tMachine->V[opcodes[X]];
		break;
	case 0x18:
		printf("LD ST %x, K\n", opcodes[X]);
		tMachine->timers[1] = tMachine->V[opcodes[X]];
		break;
	case 0x1E:
		printf("ADD I %x\n", opcodes[X]);
		tMachine->I += tMachine->V[opcodes[X]];
		break;
	case 0x29:
		printf("LD F %x\n", opcodes[X]);
		tMachine->I = 0x50 + (tMachine->V[opcodes[X] & 0xF]) * 5;
		break;
	case 0x33:
		printf("LD B %x\n", opcodes[X]);
		tMachine->V[tMachine->I + 2] = tMachine->V[opcodes[X]] % 10;
		tMachine->V[tMachine->I + 1] = (tMachine->V[opcodes[X]] / 10) % 10;
		tMachine->V[tMachine->I + 0] = (tMachine->V[opcodes[X]] / 100);
		break;
	case 0x55:
		printf("ADD [I] %x\n", opcodes[X]);
		//LD[I],X
		for (int reg = 0; reg <= opcodes[X]; reg++)
		{
			tMachine->mem[tMachine->I + reg] = tMachine->V[reg];
		}
		break;
	case 0x65:
		printf("ADD %x I \n", opcodes[X]);
		for (int reg = 0; reg <= opcodes[X]; reg++)
		{
			tMachine->V[tMachine->I + reg] = tMachine->mem[tMachine->I + reg];
		}
		break;
	}
}
