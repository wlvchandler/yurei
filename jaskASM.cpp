#include "jaskASM.h"

#include <filesystem>

using namespace std;


int main() {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path filePath = cwd / "../test/test.jsrc";
    
    try {
        logger::info << "Initializing Assembler" << std::endl;
        Assembler().assemble(filePath.string());
    } catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    //logger::info << "Initializing Runtime" << std::endl;
    //Runtime app;
    //app.load_application();
    //app.start();
    return 0;
}
