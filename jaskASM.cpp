#include "jaskASM.h"
#include "utility/log.h"
#include "assembler.h"
#include "runtime.h"

#include <iostream>

using namespace std;

int main() {
    logger::info << "Initializing Assembler" << std::endl;
    Assembler ASM;   

    logger::info << "Initializing Runtime" << std::endl;
    Runtime app;
    app.load_application();
    app.start();
    return 0;
}