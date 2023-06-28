#pragma once

#include "utility/sysinfo.h"
#include "isa.h"
#include <cstdint>
#include <fstream>
#include <map>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

enum class TokenType {
	Opcode, Reg, ImmAddr, None, Label
};

struct Token {
	TokenType type = TokenType::None;
	std::variant<std::string, uint16_t> value;
	bool ia = false; // indirect address
	Token() : value(std::string("")) {}
};

struct ExpectedOperands {
	std::vector<std::pair<TokenType, TokenType>> validCombinations;
};

struct InstructionLine {
	Token mnemonic;
	std::vector<Token> operands = {Token(), Token()};
};


class Assembler {
	uint16_t current_address = 0;
	std::unordered_map<std::string, uint16_t> symbolTable;
	std::vector<InstructionLine> instructions;
	InstructionLine current_ins_line;
	std::vector<uint16_t> binaryOut;

	void generateBinary();
	void assign(const Token&);
	void tokenize(const std::string&); 
	void parseToken(std::string&);
	bool validateOperands();
	void writeBOM(std::ofstream&);
	void writeData(std::ofstream&, std::vector<uint16_t>&);

public:
	void assemble(const std::string&);
	void writeBinary(const std::string&);

private:

	const std::unordered_map<std::string, Mnemonic> opcodes =
	{
		{"NOP",  Mnemonic::NOP},  {"HALT",  Mnemonic::HALT},
		{"LOAD", Mnemonic::LOAD}, {"STORE", Mnemonic::STORE}, {"MOV", Mnemonic::MOV}, {"PUSH", Mnemonic::PUSH}, {"POP", Mnemonic::POP},
		{"ADD",  Mnemonic::ADD},  {"SUB",   Mnemonic::SUB},   {"MUL", Mnemonic::MUL}, {"SMUL", Mnemonic::SMUL}, {"DIV",  Mnemonic::DIV}, {"SDIV",  Mnemonic::SDIV},
		{"AND",  Mnemonic::AND},  {"OR",    Mnemonic::OR},    {"XOR", Mnemonic::XOR}, {"NOT",  Mnemonic::NOT},  {"SHR", Mnemonic::SHR},  {"SHL", Mnemonic::SHL},
		{"JUMP", Mnemonic::JUMP}, {"JZ",    Mnemonic::JZ},    {"JNZ", Mnemonic::JNZ}, {"JC",   Mnemonic::JC},   {"JNC", Mnemonic::JNC},
		{"CALL", Mnemonic::CALL}, {"RET",   Mnemonic::RET},
		{"A",    Mnemonic::A},    {"B",     Mnemonic::B},     {"C",   Mnemonic::C},   {"D",    Mnemonic::D}, {"I",   Mnemonic::I},   {"J",    Mnemonic::J},
		{"SP",   Mnemonic::SP},   {"PC",    Mnemonic::PC}, {"FLAGS",   Mnemonic::FLAGS}
	};

	const std::unordered_map<std::string, ExpectedOperands> expectedOperands = {
		{ "NOP",   {{{TokenType::None, TokenType::None}}}},
		{ "HALT",  {{{TokenType::None, TokenType::None}}}},
		{ "LOAD",  {{{TokenType::Reg, TokenType::ImmAddr}}}},    // mem -> reg
		{ "STORE", {{{TokenType::Reg, TokenType::ImmAddr}}}},    // reg -> mem // TODO: swap operand order?
		{ "MOV",   {{{TokenType::Reg, TokenType::Reg}, {TokenType::Reg, TokenType::ImmAddr}}}},
		{ "PUSH",  {{{TokenType::Reg, TokenType::None}}}},
		{ "POP",   {{{TokenType::Reg, TokenType::None}}}},
		{ "ADD",   {{{TokenType::Reg, TokenType::Reg}}}},
		{ "SUB",   {{{TokenType::Reg, TokenType::Reg}}}},
		{ "MUL",   {{{TokenType::Reg, TokenType::Reg}}}},
		{ "SMUL",  {{{TokenType::Reg, TokenType::Reg}}}},
		{ "DIV",   {{{TokenType::Reg, TokenType::Reg}}}},
		{ "SDIV",  {{{TokenType::Reg, TokenType::Reg}}}},
		{ "AND",   {{{TokenType::Reg, TokenType::Reg}}}},
		{ "OR",    {{{TokenType::Reg, TokenType::Reg}}}},
		{ "XOR",   {{{TokenType::Reg, TokenType::Reg}}}},
		{ "NOT",   {{{TokenType::Reg, TokenType::None}}}},
		{ "SHR",   {{{TokenType::Reg, TokenType::None}}}},
		{ "SHL",   {{{TokenType::Reg, TokenType::None}}}},
		{ "JUMP",  {{{TokenType::ImmAddr, TokenType::None}}}},
		{ "JZ",    {{{TokenType::ImmAddr, TokenType::None}}}},
		{ "JNZ",   {{{TokenType::ImmAddr, TokenType::None}}}},
		{ "JC",    {{{TokenType::ImmAddr, TokenType::None}}}},
		{ "JNC",   {{{TokenType::ImmAddr, TokenType::None}}}},
		{ "CALL",  {{{TokenType::ImmAddr, TokenType::None}}}},
		{ "RET",   {{{TokenType::None, TokenType::None}}}}
	};
};