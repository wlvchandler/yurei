#include "assembler.h"

void Assembler::generateBinary(const std::string& f) {
	std::istringstream iss(f);
	std::string token;

    while (iss >> token) {
        if (token.empty()) {
            continue;
        }

        if (token.back() == ',') {
            token.pop_back();
        }

        bool ia = false;
        if (token.front() == '[' && token.back() == ']') {
            token = token.substr(1, token.size() - 2);
            ia = true;
        }

        auto it = opcodes.find(token);
        if (it != opcodes.end()) {
            binaryOut.push_back(it->second | (ia ? 0x8000 : 0));
        } else {
            try {
                binaryOut.push_back(std::stoi(token));
            } catch (const std::exception& e) {
                std::cerr << "Error converting stoi: " << e.what() << std::endl; //TODO: logging
            }
        }
    }
}

void Assembler::resolveSymbols() {
}

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
} // std::ofstream outgb("app.jgb", std::ios::binary)