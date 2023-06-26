#pragma once

#include "utility/sysinfo.h"
#include "isa.h"
#include <cstdint>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class Opcode;

struct InstructionLine {
  Opcode opcode;
  std::vector<std::string> operands;
};

enum class TokenType {
	Opcode, Register, ImmediateValue, MemoryAddress, Unknown
};

struct Token {
	TokenType type;
	std::string value;
};

class Assembler {
	void generateBinary();
	std::vector<std::string> tokenize(const std::string&);
	void resolveSymbols();

	std::string filename;

public:
	void assemble(const std::string&);
	void writeBinary(const std::string&);

private:
	std::vector<std::string> pass1_tokens;
	std::vector<Instruction> instructions;
	std::vector<uint16_t> binaryOut;
	std::unordered_map<std::string, uint16_t> symbolTable;

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

};
