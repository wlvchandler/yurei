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
#include <variant>

class Opcode;



enum class TokenType {
	Opcode, Reg, ImmAddr, None, Label
};

struct Token {
	TokenType type = TokenType::None;
	std::variant<std::string, uint16_t> value;
	bool ia; // indirect address
};

struct ExpectedOperands {
	TokenType operand1;
	TokenType operand2;
};

struct InstructionLine {
	Token mnemonic;
	std::vector<Token> operands;
};


class Assembler {
	void generateBinary();
	std::vector<std::string> tokenize(const std::string&); 
	void parseToken(std::string);
	bool validateOperands();

	std::string filename;

public:
	void assemble(const std::string&);
	void writeBinary(const std::string&);

private:
	std::unordered_map<std::string, uint16_t> symbolTable;
	std::vector<std::string> pass1_tokens;
	std::vector<InstructionLine> instructions;
	InstructionLine current_ins_line;
	std::vector<uint16_t> binaryOut;

	const std::unordered_map<std::string, Mnemonic> opcodes =
	{
		{"NOP",  Mnemonic::NOP},  {"HALT",  Mnemonic::HALT},
		{"LOAD", Mnemonic::LOAD}, {"STORE", Mnemonic::STORE}, {"MOV", Mnemonic::MOV}, {"PUSH", Mnemonic::PUSH}, {"POP", Mnemonic::POP},
		{"ADD",  Mnemonic::ADD},  {"SUB",   Mnemonic::SUB},   {"MUL", Mnemonic::MUL}, {"DIV",  Mnemonic::DIV},
		{"AND",  Mnemonic::AND},  {"OR",    Mnemonic::OR},    {"XOR", Mnemonic::XOR}, {"NOT",  Mnemonic::NOT},
		{"JUMP", Mnemonic::JUMP}, {"JZ",    Mnemonic::JZ},    {"JNZ", Mnemonic::JNZ}, {"JC",   Mnemonic::JC},   {"JNC", Mnemonic::JNC},
		{"CALL", Mnemonic::CALL}, {"RET",   Mnemonic::RET},
		{"A",    Mnemonic::A},    {"B",     Mnemonic::B},     {"C",   Mnemonic::C},   {"D",    Mnemonic::D}
	};

	const std::unordered_map<std::string, ExpectedOperands> expectedOperands = {
		{"NOP",   {TokenType::None, TokenType::None}},
		{"HALT",  {TokenType::None, TokenType::None}},
		{"LOAD",  {TokenType::Reg,	TokenType::ImmAddr}},    // mem -> reg
		{"STORE", {TokenType::Reg,	TokenType::ImmAddr}},    // reg -> mem // TODO: swap operand order?
		{"MOV",   {TokenType::Reg,	TokenType::Reg}},        // reg -> reg
		{"PUSH",  {TokenType::Reg,	TokenType::None}},
		{"POP",   {TokenType::Reg,  TokenType::None}},
		{"ADD",   {TokenType::Reg,  TokenType::Reg}},
		{"SUB",   {TokenType::Reg,  TokenType::Reg}},
		{"MUL",   {TokenType::Reg,  TokenType::Reg}},
		{"SMUL",  {TokenType::Reg,  TokenType::Reg}},
		{"DIV",   {TokenType::Reg,  TokenType::Reg}},
		{"SDIV",  {TokenType::Reg,  TokenType::Reg}},
		{"AND",   {TokenType::Reg,  TokenType::Reg}},
		{"OR",    {TokenType::Reg,  TokenType::Reg}},
		{"XOR",   {TokenType::Reg,  TokenType::Reg}},
		{"NOT",   {TokenType::Reg,  TokenType::None}},
		{"SHR",   {TokenType::Reg,  TokenType::None}},
		{"SHL",   {TokenType::Reg,  TokenType::None}},
		{"JUMP",  {TokenType::ImmAddr, TokenType::None}},
		{"JZ",    {TokenType::ImmAddr, TokenType::None}},
		{"JNZ",   {TokenType::ImmAddr, TokenType::None}},
		{"JC",    {TokenType::ImmAddr, TokenType::None}},
		{"JNC",   {TokenType::ImmAddr, TokenType::None}},
		{"CALL",  {TokenType::ImmAddr, TokenType::None}},
		{"RET",   {TokenType::None, TokenType::None}}
	};

};
