#pragma once

#include "utility/sysinfo.h"
#include "isa.h"
#include "memory.h"
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class Memory;
class Opcode;

using namespace REGISTERS;



//enum class Instruction : unsigned char {
//	NOP, HALT, LOAD, STORE, MOV, ADD, SUB, MUL, DIV, AND, OR, XOR, NOT,
//	JMP, JZ, JNZ, JC, JNC, CALL, RET, PUSH, POP, N_OPCODES
//};

//struct Instruction {
//	Opcode opcode;
//	std::vector<std::string> operands;
//};

class Assembler {
	//void tokenize(const std::string&); 
	//void generateBinary(Instruction&, std::ostream&);
	void generateBinary(const std::string&);
	void resolveSymbols();

	std::string filename;

public:
	void parse(std::istream&); // std::ifstream in("input.jasm");
	void writeBinary(const std::string&);

private:
	std::vector<Instruction> instructions;
	std::vector<uint16_t> binaryOut;

    const std::unordered_map<std::string, uint16_t> opcodes = 
    {
		// control
        {"NOP",  0x0000}, {"HALT",  0x0001},
		// memory
		{"LOAD", 0x000A}, {"STORE", 0x000B}, {"MOV",  0x000C}, {"PUSH", 0x000D}, {"POP",  0x000E},
		// arithmetic
		{"ADD",  0x0010}, {"SUB",   0x0011}, {"MUL",  0x0012}, {"DIV",  0x0013},
		// logical
		{"AND",  0x0020}, {"OR",    0x0021}, {"XOR",  0x0022}, {"NOT",  0x0023},
		// jumps
		{"JUMP", 0x0030}, {"JZ",    0x0031}, {"JNZ",  0x0032}, {"JC",   0x0033}, {"JNC",  0x0034},
		// subroutine
		{"CALL", 0x0040}, {"RET",   0x0041},
		// registers
        {"A",    0x00F0}, {"B",     0x00F1}, {"C",    0x00F2}, {"D",    0x00F3}, 
		//{"E",    0x00F4}, {"F",     0x00F5}, {"G",    0x00F6}
    };
	std::map<std::string, int> symbolTable;
};