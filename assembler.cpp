#include "assembler.h"

static void writeBOM(std::ofstream& ofs) {
    uint16_t BOM = isSystemLittleEndian() ? 0xFFFE : 0xFEFF;
    ofs.write(reinterpret_cast<const char*>(&BOM), sizeof(BOM));
}

void Assembler::writeBinary(const std::string& output_file) {
    this->filename = output_file;
    std::ofstream out(output_file, std::ios::binary);
    for (auto& byte : this->binaryOut) {
        out << byte;
    }
    out.close();
}

void Assembler::parseTokens(std::vector<Token>& const tokens) {

    for (auto& token : tokens) {
        if (token.value.empty()) {
            continue;
        }

        if (token.value.back() == ',') {
            token.value.pop_back();
        }

        bool ia = false;
        if (token.value.front() == '[' && token.value.back() == ']') {
            token.value = token.value.substr(1, token.value.size() - 2);
            ia = true;
        }
        auto it = opcodes.find(token.value);
        if (it != opcodes.end()) {
            binaryOut.push_back(static_cast<uint16_t>(it->second) | (ia ? 0x8000 : 0));
        }
        else {
            try {
                binaryOut.push_back(std::stoi(token.value));
            }
            catch (const std::exception& e) {
                std::cerr << "Error converting stoi: " << e.what() << std::endl; //TODO: logging
            }
        }
    }
}

std::vector<Token> Assembler::tokenize(const std::string& line) {
    std::vector<Token> tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {

    }
    return tokens;
}

void Assembler::generateBinary(const std::string& f) {
    std::istringstream iss(f);
	std::string line;

    while (std::getline(iss, line)) {
        auto tokens = tokenize(line);
        parseTokens(tokens);
    }

}

void Assembler::resolveSymbols() {
}
