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

void Assembler::generateBinary() {


    for (auto& instruction : instructions) {
        // handle mnemonic
        if (instruction.mnemonic.type == TokenType::Opcode) {
            auto expected = expectedOperands.find(std::get<std::string>(instruction.mnemonic.value));
            if (expected == expectedOperands.end()) {
                std::cerr << "Unknown mnemonic\n";
                return;
            }
        }
        //in progress

        for (auto& operand : instruction.operands) {
            switch (operand.type) {
            case TokenType::Opcode:
                break;
            case TokenType::Reg:
                break;
            case TokenType::ImmAddr:
                break;
            case TokenType::Label:
                break;
            default:
                break;
            };
        }
    }

    for (auto& token : pass1_tokens) {
        if (token.empty()) {
            continue;
        }

        // remove possible indirect addressing operator
        bool ia = false;
        if (token.front() == '[' && token.back() == ']') {
            token = token.substr(1, token.size() - 2);
            ia = true;
        }

        // immediate value/address [TODO: handle postfix notation]
        int16_t lit;
        if (isNumber(token, lit)) {
            binaryOut.push_back(lit);
            continue;
        }

        // Label [TODO: make sure labels are not the same name  as any known mnemonic]
        if (symbolTable.find(token) != symbolTable.end()) {
            binaryOut.push_back(symbolTable[token]);
            continue;
        }
        
        // Register TODO: separate from opcodes structure ?  
                       
        // Known mnemonic
        auto it = opcodes.find(token);
        if (it != opcodes.end()) {
            binaryOut.push_back(static_cast<uint16_t>(it->second) | (ia ? 0x8000 : 0));
        } else {
            try {
                binaryOut.push_back(std::stoi(token));
            }
            catch (const std::exception& e) {
                std::cerr << "Error converting stoi: " << e.what() << std::endl; //TODO: logging
            }
        }
    }
}


 bool Assembler::validateInstructionLine() {
     return true;
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
        //for (auto s : tokenize(line)){
        //    pass1_tokens.push_back(s);
        //}
    }

    generateBinary(); 
    writeBinary("../test/out.j16");

    file.close();

}
