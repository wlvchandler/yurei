#include "assembler.h"

// byte order mark
void Assembler::writeBOM(std::ofstream& ofs) {
    uint16_t BOM = isSystemLittleEndian() ? 0xFFFE : 0xFEFF;
    ofs.write(reinterpret_cast<const char*>(&BOM), sizeof(BOM));
}

void Assembler::writeData(std::ofstream& ofs, std::vector<uint16_t>& data) {
    ofs.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(uint16_t));
}

void Assembler::writeBinary(const std::string& output_file) {
    std::ofstream out(output_file, std::ios::binary);
    if (out.is_open()){
        writeBOM(out);
        writeData(out, binaryOut);
    } else {
        throw std::runtime_error("j16 error: could not open file to write: " + output_file);
    }
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
    std::string& mnemonic = std::get<std::string>(current_ins_line.mnemonic.value);
    auto expected = expectedOperands.find(mnemonic);
    if (expected == expectedOperands.end()) {
        throw std::runtime_error("j16 error: unknown mnemonic: `" + mnemonic + "`");
    }

    Token& op1 = current_ins_line.operands.front();
    if (op1.type == TokenType::Label) {
        const std::string& label = std::get<std::string>(op1.value);
        auto label_it = symbolTable.find(label);
        if (label_it == symbolTable.end()) {
            throw std::runtime_error("j16 error: unknown label: `" + label + "`");
        }
        op1.value = label_it->second;
        op1.type = TokenType::ImmAddr;
    }

    const Token& op2 = current_ins_line.operands.back();

    for (const auto& opcombo : expected->second.validCombinations) {
        if (opcombo.first == op1.type && opcombo.second == op2.type) {
            return true;
        }
    }
    
    throw std::runtime_error("j16 error: invalid operands for `" + mnemonic + "`");
}

template<typename M>
static void _generate(const M& map, const std::string& k, std::vector<uint16_t>& output, uint16_t mask = 0) {
    auto it = map.find(k);
    if (it == map.end()) {
        throw std::runtime_error("j16 error: parse: unknown or invalid token: `" + k + "`");
    }
    output.emplace_back((static_cast<uint16_t>(it->second) | mask));
}

void Assembler::generateBinary() {
    for (auto& instruction : instructions) {
        current_ins_line = instruction;

        if (instruction.mnemonic.type == TokenType::Opcode && validateOperands()) {

            _generate(opcodes, std::get<std::string>(instruction.mnemonic.value), binaryOut);

            for (auto& operand : instruction.operands) {
                switch (operand.type) {
                case TokenType::None:
                    break;
                case TokenType::ImmAddr:
                    binaryOut.emplace_back(std::get<uint16_t>(operand.value));
                    break;
                case TokenType::Reg:
                    _generate(opcodes, std::get<std::string>(operand.value), binaryOut, (operand.ia ? 0x8000 : 0));
                    break;
                case TokenType::Label:
                    _generate(symbolTable, std::get<std::string>(operand.value), binaryOut);
                    break;
                default:
                    throw std::runtime_error("j16 error: unknown operand type for " + std::get<std::string>(instruction.mnemonic.value));
                };
            }
        }
    }
}

void Assembler::assign(const Token& t) {
    if (t.type == TokenType::Opcode) {
        current_ins_line.mnemonic = t;
    } else {
        current_ins_line.operands[!(current_ins_line.operands[0].type == TokenType::None)] = t;
    }
    current_address++;
}

 void Assembler::parseToken(std::string& token) {
    Token t;
    // strip indirect address formatting
    if (token.front() == '[' && token.back() == ']') {
        token = token.substr(1, token.size() - 2);
        t.ia = true;
    }
    t.value = token;

    // known opcode or register
    auto it = opcodes.find(token);
    if (it != opcodes.end()) {
        t.type = TokenType::Opcode;
        if (static_cast<uint16_t>(it->second) >= 0x00F0) {
            t.type = TokenType::Reg;
        }
        assign(t);
        return;
    }
    
    // number (immediate/address)
    int16_t immaddr;
    if (isNumber(token, immaddr)) {
        t.type = TokenType::ImmAddr;
        t.value = static_cast<uint16_t>(immaddr);
        assign(t);
        return;
    }    

    // assume it's a label
    t.type = TokenType::Label;
    assign(t);
}

 void Assembler::tokenize(const std::string& line) {
     std::stringstream ss(line);
     std::string token;
     current_ins_line = {};

     while (ss >> token) {
         // comments
         size_t comment_idx = token.find(';');
         if (comment_idx != std::string::npos) {
             if (comment_idx == 0) { break; }
             token.resize(comment_idx);
         }

         // label
         size_t label_idx = token.find(':');
         if (label_idx != std::string::npos) {
             symbolTable[token.substr(0, label_idx)] = current_address;
             token = token.substr(label_idx + 1);
             if (token.empty()) { continue; }
         }

         if (token.back() == ',') {
             token.pop_back();
         }

         parseToken(token);
     }
 }

void Assembler::assemble(const std::string& f) {
    std::ifstream file(f);
    if (!file.is_open()){
        throw std::runtime_error("j16 error: failed to open file " + f);
    }

    std::string line;
    while (std::getline(file, line)) {
        tokenize(line);
        if (current_ins_line.mnemonic.type != TokenType::None) {
            instructions.emplace_back(current_ins_line);
        }
    }
    generateBinary();
    writeBinary("../test/out.j16");

    // DEBUG -- print out generated code
    int i = 0;
    for (auto& b : binaryOut) {
        std::cout << std::setw(4) << std::setfill('0') << std::hex << b << " ";
        if (i++ == 15) {
            std::cout << std::endl;
            i = 0;
        }
    }
    while (i > 0 && i < 16) {
        std::cout << std::setw(4) << std::setfill('0') << std::hex << 0 << " ";
        i++;
    }
    std::cout << std::endl;
    // END DEBUG
}
