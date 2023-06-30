#include <filesystem>
#include <j16/assembler.h>
#include <j16/j16.h>

using namespace std;
using namespace oni16;

int main(int argc, char** argv) {

    try {
        std::vector<std::string> args(argv + 1, argv + argc);
        oni16::parseOptions(args);

        // std::filesystem::path cwd = std::filesystem::current_path();
        // std::filesystem::path filePath = cwd / oni16_options.inputFile;

        Assembler().assemble();
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}