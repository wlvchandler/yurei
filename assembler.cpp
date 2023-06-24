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

void Assembler::parseTokens(std::vector<Token>& tokens) {

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


static void saveToken(std::string& token, std::vector<std::string>& token_list) {
  token_list.push_back(token);
  token.clear();
}

std::vector<std::string> Assembler::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::string currentToken;
    for (char c : line + ' ') {
      if (c == ';') {
	if (!currentToken.empty()) {
	  saveToken(currentToken, tokens);
	}
	break;
      }
      if ((c == ',' || isspace(c)) && !currentToken.empty()) {
	saveToken(currentToken, tokens);
	continue;
      }
      if (isspace(c)) { continue; }
      currentToken.push_back(c);
    }

    
    return tokens;
}

void Assembler::generateBinary(const std::string& f) {
  std::ifstream file(f);
  if (!file.is_open()){
    std::cerr << "Failed to open file\n";
    return;
  }
  std::string line;
  std::vector<std::string> all_tokens;
  while (std::getline(file, line)) {
    auto tokens = tokenize(line);
    for (auto s : tokens){
      all_tokens.push_back(s);
    }
    //parseTokens(tokens);
  }

  file.close();
}

void Assembler::resolveSymbols() {
}
