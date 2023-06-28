#include "assembler.h"

// byte order mark
static void writeBOM(std::ofstream& ofs) {
    uint16_t BOM = isSystemLittleEndian() ? 0xFFFE : 0xFEFF;
    ofs.write(reinterpret_cast<const char*>(&BOM), sizeof(BOM));
}

static void writeData(std::ofstream& ofs, std::vector<uint16_t>& data) {
    ofs.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(uint16_t));
}

void Assembler::writeBinary(const std::string& output_file) {
    this->filename = output_file;
    std::ofstream out(output_file, std::ios::binary);
    if (out.is_open()){
        writeBOM(out);
        writeData(out, binaryOut);
    } else {
        std::cerr << "Could not open file to write\n";
    }
    out.close();
}

static bool isNumber(const std::string& token, int16_t& value) {
    bool isValid = true;
    try {
        std::string localToken = token;
        short sign = 1;
        if (token.front() == '-') {
            sign = -1;
            localToken = token.substr(1);
        }
        bool hex = (localToken.find("0x") == 0 || localToken.find("0X") == 0);
        bool oct = (localToken.find("0o") == 0 || localToken.find("0O") == 0);
        bool bin = (localToken.find("0b") == 0 || localToken.find("0B") == 0);
        if (hex || oct || bin) {
            localToken = localToken.substr(2);
        }
        size_t pos;
        uint16_t _v = std::stoul(localToken.c_str(), &pos, hex ? 16 : (oct ? 8 : (bin ? 2 : 10)));
        if (pos != localToken.size() || _v > (sign == -1 ? 0x8000 : 0xFFFF)) {
            isValid = false;
        } else {
            value = static_cast<int16_t>(sign * _v);
        }
    } catch (std::invalid_argument&) {
        isValid = false;
    } catch (std::out_of_range&) {
        isValid = false;
    }
    return isValid;
}

static std::string GetTokenType(TokenType tt) {
    return (tt == TokenType::Opcode ? "Opcode" :
        (tt == TokenType::Reg ? "Register" :
            (tt == TokenType::ImmAddr ? "ImmAddr" :
                (tt == TokenType::Label ? "Label" : "NONE"))));
}

bool Assembler::validateOperands() {
    bool valid = true;
    std::string& mnemonic = std::get<std::string>(current_ins_line.mnemonic.value);
    auto expected = expectedOperands.find(mnemonic);
    if (expected == expectedOperands.end()) {
        std::cerr << "Unknown mnemonic\n";
        valid = false;
    }

    if (valid) {
        valid = false;

        Token op1 = current_ins_line.operands.front();
        Token op2 = current_ins_line.operands.back();

        //correct for labels
        if (op1.type == TokenType::Label) {
            op1.value = symbolTable[std::get<std::string>(current_ins_line.operands.front().value)];
            op1.type = TokenType::ImmAddr;
        }

        for (const auto& opcombo : expected->second.validCombinations) {
            if (opcombo.first == op1.type && opcombo.second == op2.type) {
                valid = true;
                break;
            }
        }
    }

    if (!valid) {
        std::cerr << "Invalid operands for mnemonic\n";
    }

    return valid;
}

static void printInstruction(InstructionLine& il) {
    std::cout << std::get<std::string>(il.mnemonic.value) << " : ";

    if (!il.operands.empty()) {
        if (il.operands.front().type != TokenType::None && il.operands.front().type == TokenType::ImmAddr) {
            std::cout << std::get<uint16_t>(il.operands.front().value);
        }
        else {
            std::cout << std::get<std::string>(il.operands.front().value);
        }
    }
    else std::cout << "-";

    std::cout << " / ";

    if (!il.operands.empty()) {
        if (il.operands.back().type == TokenType::ImmAddr) {
            std::cout << std::get<uint16_t>(il.operands.back().value);
        }
        else {
            std::cout << std::get<std::string>(il.operands.back().value);
        }
    }
    else std::cout << "-";

    std::cout << std::endl;
}



void Assembler::generateBinary() {
    current_ins_line = {};
    for (auto& instruction : instructions) {
        current_ins_line = instruction;

        if (instruction.mnemonic.type == TokenType::Opcode && validateOperands()) {
            std::string& mnemonic = std::get<std::string>(instruction.mnemonic.value);
            uint16_t opcode = static_cast<uint16_t>(opcodes.at(mnemonic));

            binaryOut.push_back(opcode);

            for (auto& operand : instruction.operands) {
                switch (operand.type) {
                case TokenType::None:
                    break;
                case TokenType::Reg:
                {
                    uint16_t regop = static_cast<uint16_t>(opcodes.at(std::get<std::string>(operand.value)));
                    binaryOut.push_back(regop | (operand.ia ? 0x8000 : 0));
                    break;
                }
                case TokenType::ImmAddr:
                {
                    uint16_t immaddr = std::get<uint16_t>(operand.value);
                    binaryOut.push_back(immaddr);
                    break;
                }
                case TokenType::Label:
                    if (symbolTable.find(std::get<std::string>(operand.value)) != symbolTable.end()) {
                        binaryOut.push_back(symbolTable[std::get<std::string>(operand.value)]);
                    }
                    break;
                default:
                    std::cerr << "Unknown operand type";
                    break;
                };
            }
        }
    }
}

 void Assembler::parseToken(std::string& token) {
    Token t;
    // strip indirect address formatting
    if (token.front() == '[' && token.back() == ']') {
        token = token.substr(1, token.size() - 2);
        t.ia = true;
    }
    t.value = token;

    bool parsed = false;

    // check if it's a known opcode or register
    if (!parsed) {
        auto it = opcodes.find(token);
        if (it != opcodes.end()) {
            t.type = TokenType::Opcode;
            if (static_cast<uint16_t>(it->second) >= 0x00F0) {
                t.type = TokenType::Reg;
            }
            parsed = true;
        }
    }

    // check if it's a number
    if (!parsed) {
        int16_t immaddr;
        if (isNumber(token, immaddr)) {
            t.type = TokenType::ImmAddr;
            t.value = static_cast<uint16_t>(immaddr);
            parsed = true;
        }
    }

    // assume it's a label
    if (!parsed) {
        t.type = TokenType::Label;
    }

    if (t.type == TokenType::Opcode) {
        current_ins_line.mnemonic = t;
    } else {
        int idx = !(current_ins_line.operands[0].type == TokenType::None);
        current_ins_line.operands[idx] = t;
    }

    current_address++;
    token.clear();
}

void Assembler::tokenize(const std::string& line) {
    std::string currentToken;

    current_ins_line = {}; // refurbish

    for (char c : line + ' ') {
        if (c == ';') {
	        if (!currentToken.empty()) {
                parseToken(currentToken);
	        }
	        break;
        }
        if (c == ':') {
            symbolTable[currentToken] = current_address;
            currentToken.clear();
            continue;
        }
        if ((c == ',' || isspace(c)) && !currentToken.empty()) {
            parseToken(currentToken);
	        continue;
        }
        if (isspace(c)) { continue; }
        currentToken.push_back(c);
    }
}

void Assembler::assemble(const std::string& f) {
    std::ifstream file(f);
    if (!file.is_open()){
        std::cerr << "Failed to open file\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        tokenize(line);
        if (current_ins_line.mnemonic.type != TokenType::None) {
            instructions.push_back(current_ins_line);
        }
    }
    generateBinary();
    writeBinary("../test/out.j16");
    file.close();

}
