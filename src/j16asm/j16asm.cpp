#include <j16/j16.h>
#include <j16/assembler.h>
#include <filesystem>

using namespace std;

int main() {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path filePath = cwd / TESTS_DIR;

    try {
        Assembler().assemble(filePath.string() + "/test.jsrc");
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
	return 0;
}