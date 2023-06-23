#pragma once

#include "utility/sysinfo.h"
#include "isa.h"
#include "memory.h"
#include <cstdint>
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


	const std::unordered_map<std::string, Instruction> opcodes =
	{
		{"NOP",  Instruction::NOP},  {"HALT",  Instruction::HALT},
		{"LOAD", Instruction::LOAD}, {"STORE", Instruction::STORE}, {"MOV", Instruction::MOV}, {"PUSH", Instruction::PUSH}, {"POP", Instruction::POP},
		{"ADD",  Instruction::ADD},  {"SUB",   Instruction::SUB},   {"MUL", Instruction::MUL}, {"DIV",  Instruction::DIV},
		{"AND",  Instruction::AND},  {"OR",    Instruction::OR},    {"XOR", Instruction::XOR}, {"NOT",  Instruction::NOT},
		{"JUMP", Instruction::JUMP}, {"JZ",    Instruction::JZ},    {"JNZ", Instruction::JNZ}, {"JC",   Instruction::JC},   {"JNC", Instruction::JNC},
		{"CALL", Instruction::CALL}, {"RET",   Instruction::RET},
		{"A",    Instruction::A},    {"B",     Instruction::B},     {"C",   Instruction::C},   {"D",    Instruction::D}
	};

	std::map<std::string, int> symbolTable;
};