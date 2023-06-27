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

bool Assembler::validateOperands() {
    bool valid = true;

    auto expected = expectedOperands.find(std::get<std::string>(current_ins_line.mnemonic.value));
    if (expected == expectedOperands.end()) {
        std::cerr << "Unknown mnemonic\n";
        valid = false;
    }

    if (valid) {
        Token op1 = current_ins_line.operands.front();
        Token op2 = current_ins_line.operands.back();

        if (!(expected->second.operand1 == op1.type && expected->second.operand2 == op2.type)) {
            std::cerr << "Invalid operands for mnemonic\n";
            valid = false;
        }
    }

    return valid;
}

void Assembler::generateBinary() {
    for (auto& instruction : instructions) {
        if (instruction.mnemonic.type == TokenType::Opcode && validateOperands()) {
            binaryOut.push_back(std::get<uint16_t>(instruction.mnemonic.value));
            for (auto& operand : instruction.operands) {
                std::string label = std::get<std::string>(operand.value);
                switch (operand.type) {
                case TokenType::Reg:
                case TokenType::ImmAddr: //fallthrough since operand.ia will always be false for ImmAddr
                    binaryOut.push_back(std::get<uint16_t>(operand.value) | (operand.ia ? 0x8000 : 0));     
                    break;
                case TokenType::Label:
                    if (symbolTable.find(label) != symbolTable.end()) {
                        binaryOut.push_back(symbolTable[label]);
                    }
                    else {
                        std::cerr << "Label " << label << " not found" << std::endl;
                    }
                    break;
                default:
                    std::cerr << "Unknown operand type" << std::endl;
                    break;
                };
            }
        }
    }
}

 void Assembler::parseToken(std::string token) {
    Token t;
    t.value = token;

    // strip indirect address formatting
    if (token.front() == '[' && token.back() == ']') {
        token = token.substr(1, token.size() - 2);
        t.ia = true;
    }

    bool parsed = false;

    // check if it's a known opcode or register
    if (!parsed) {
        auto it = opcodes.find(token);
        if (it != opcodes.end()) {
            t.type = TokenType::Opcode;
            if (static_cast<uint16_t>(it->second) > 0x00F0) {
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
    
    if (current_ins_line.mnemonic.type == TokenType::None) {
        current_ins_line.mnemonic = t;
    } else {
        current_ins_line.operands.push_back(t);
    }
}

std::vector<std::string> Assembler::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::string currentToken;

    current_ins_line.mnemonic = {}; // refurbish

    static uint16_t current_address = 0;
    bool isLabel = false;

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
            isLabel = true;
            continue;
        }
        if ((c == ',' || isspace(c)) && !currentToken.empty()) {
            parseToken(currentToken);
	        continue;
        }
        if (isspace(c)) { continue; }
        currentToken.push_back(c);
    }

    if (!isLabel) {
        current_address++;
    }
        
    return tokens;
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
    }
    generateBinary(); 

    std::cout << std::hex;
    for (auto b : binaryOut) {
        std::cout <<  b << " ";
    }
    std::cout << "\n";

    writeBinary("../test/out.j16");
    file.close();

}
