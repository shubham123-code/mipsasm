#include "stdc++.h"
#include <fstream>
#define li long int
using namespace std;

ifstream myfile;
string file;
ofstream output;
vector<li>instructions;
int SP, PC, A, B;

// Memory with 2^24 addresses
int memory[16777216]; 

char* mnemonic[] = {"ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub", "shl", "shr", "adj", "a2sp", "sp2a", "call", "return", "brz", "brlz", "br", "HALT"};

void read_file()
{
    char y[100];
    if (!myfile)
    {
        cout << "file is empty";
        return;
    }
    int line;

    while (myfile.read((char *)&line, sizeof(line))){
        instructions.push_back(line);
    }
}

void ldc(int value) {
	B = A;
	A = value;
}

void adc(int value) {
	A += value;
}

void ldl(int offset) {
	B = A;
	A = memory[SP + offset];
}

void stl(int offset) {
	memory[SP + offset] = A;
	A = B;
}

void ldnl(int offset) {
	A = memory[A + offset];
}

void stnl(int offset) {
	memory[A + offset] = B;
}

void add() {
	A += B;
}

void sub() {
	A = B - A;
}

void shl() {
	A = B << A;
}

void shr() {
	A = B >> A;
}

void adj(int value) {
	SP += value;
}

void a2sp() {
	SP = A;
	A = B;
}

void sp2a() {
	B = A;
	A = SP;
}

void call(int offset) {
	B = A;
	A = PC;
	PC += offset;
}

void RETURN() {
	PC = A;
	A = B;
}

void brz(int offset) {
	if (A == 0) PC += offset;
}

void brlz(int offset) {
	if (A < 0) PC += offset;
}

void br(int offset) {
	PC += offset;
} 
char* getType(int mnem) {
	if (mnem < 4 || mnem == 10) return "value";
	else if (mnem == 4 || mnem == 5) return "value";
	else if (mnem == 15 || mnem == 17) return "offset";
	else if (mnem == 13 || mnem == 16) return "offset";
	return "";
}

// Print ISA
void isa() {
	fprintf(stderr, "Opcode\tMnemonic\tOperand\n");
	for (int i = 0; i <= 18; i++) {
		fprintf(stderr, "%d\t%s\t%s\n", i, mnemonic[i], getType(i));
	}
	fprintf(stderr, "\tSET\tvalue\n");
	return;
}

// Memory Dump until the number on instructions/data provided
void printMemory(int line_num, bool isBefore) {
	if (isBefore) fprintf(stderr, "\nMemory dump before execution\n");
	else fprintf(stderr, "\nMemory dump after execution\n");
	for (int i = 0; i < line_num; i++) {
		if (i % 4 == 0) fprintf(stderr, "\n%08x ", i);
		fprintf(stderr, "%08x ", memory[i]);
	}
	fprintf(stderr, "\n");
	return;
}

// Memory Dump until the number on instructions/data provided
void printMemoryToFile(int line_num, ofstream& file) {
	for (int i = 0; i < line_num; i++) {
		if (i % 4 == 0) file<<"\n%08x "<<i;
		file<<"%08x "<<memory[i];
	}
	file<<"\n";
	return;
}

// Convert 32 bit binary string to number
int binToNumber(char* bin) {
	int number = 0;
	for (int i = 0; i < 32; i++) {
		number *= 2;
		if (bin[i] == '1') number += 1; 
	}
	return number;
}

// Read machine code into memory


// Extract mnemonic code from instruction
int getMnemonicCode(int instruction) {
	return ((1 << 8) - 1) & instruction;
}

// Extract mnemonic code from instruction
int getOperand(int instruction) {
	return (instruction >> 8);
}

// Prints trace details
void printTrace(int opcode, int operand) {
	fprintf(stderr, "PC=%08x, SP=%08x, A=%08x, B=%08x %s\t%08x\n", PC, SP, A, B, mnemonic[opcode], operand);
}

// Print executable options
void printOptions() {
	fprintf(stderr, "usage: emu [options] file.o\n");
	fprintf(stderr, "-trace\tshow instruction trace\n");
	fprintf(stderr, "-before\tshow memory dump before execution\n");
	fprintf(stderr, "-after\tshow memory dump after execution\n");
	fprintf(stderr, "-isa\tdisplay ISA\n");
}

// Maps opcode to required function
int executeInstruction(int opcode, int operand) {
	if (opcode == 0) ldc(operand);
	if (opcode == 1) adc(operand);
	if (opcode == 2) ldl(operand);
	if (opcode == 3) stl(operand);
	if (opcode == 4) ldnl(operand);
	if (opcode == 5) stnl(operand);
	if (opcode == 6) add();
	if (opcode == 7) sub();
	if (opcode == 8) shl();
	if (opcode == 9) shr();
	if (opcode == 10) adj(operand);
	if (opcode == 11) a2sp();
	if (opcode == 12) sp2a();
	if (opcode == 13) call(operand);
	if (opcode == 14) RETURN();
	if (opcode == 15) brz(operand);
	if (opcode == 16) brlz(operand);
	if (opcode == 17) br(operand);
	else return 0;
	return 1;
}
int execute(bool trace) {
	SP = (1 << 23) -1;
	int operand = 0, opcode = 0;
	int instruction_num = 0;

	while ((opcode = getMnemonicCode(memory[PC])) <= 18) {
		
		operand = getOperand(memory[PC]);

		if (trace) printTrace(opcode, operand);
		if (opcode == 18) break;
		// Invalid Opcode
		if (opcode > 18) return 1;

		PC += 1;
		executeInstruction(opcode, operand);
		instruction_num += 1;
	}
	fprintf(stderr, "\n%d instructions executed.\n", instruction_num);
	return 0;
}

int main(int argc, char *argv[])
{
    myfile.open(argv[2], ios::binary | ios::in);
    output.open("output.txt");
    read_file();

	

	// If no arguments
	if (argc < 3) {
		printOptions();
		return 1;
	}

	char* file_name = (char*) malloc(1024);
	strcpy(file_name, argv[2]);
	char* ptr = strchr(file_name, '.');
	if (ptr != NULL) *ptr = '\0';

	char* out_name = (char*) malloc(1024);
	strcat(out_name, file_name);
	strcat(out_name, "_memdump.txt");
	

	bool trace = (strcmp(argv[1], "-trace") == 0)? 1: 0;
	bool before = (strcmp(argv[1], "-before") == 0)? 1: 0;
	bool after = (strcmp(argv[1], "-after") == 0)? 1: 0;
	bool ISA = (strcmp(argv[1], "-isa") == 0)? 1: 0;

	// Invalid option
	if (!(trace || before || after || ISA)) {
		printOptions();
		return 1;
	}

	int instructions_cnt = instructions.size();
	// If -before then print
	if (before) printMemory(instructions_cnt, 1);
	if (ISA) isa();
    int PC=0,t=0;
    for(int u:instructions){
        memory[t++]=u;
    }
	execute(trace);

	if (after) printMemory(instructions_cnt, 0);
	printMemoryToFile(instructions_cnt, output);
    myfile.close();
}