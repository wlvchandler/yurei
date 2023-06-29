#include <j16/jaskASM.h>

#include <filesystem>

using namespace std;


int main() {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path filePath = cwd / TESTS_DIR;
    std::cout << filePath << std::endl;
        
    try {
        logger::info << "Initializing Assembler" << std::endl;
        Assembler().assemble(filePath.string() + "/test.jsrc");
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    //logger::info << "Initializing Runtime" << std::endl;
    //Runtime app;
    //app.load_application();
    //app.start();
    return 0;
}
