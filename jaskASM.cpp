#include "jaskASM.h"

#include <filesystem>

using namespace std;


int main() {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path filePath = cwd / "../test/test.jsrc";

    logger::info << "Initializing Assembler" << std::endl;
    Assembler ASM;   
    ASM.assemble(filePath.string());

    //logger::info << "Initializing Runtime" << std::endl;
    //Runtime app;
    //app.load_application();
    //app.start();
    return 0;
}
