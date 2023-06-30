#include <j16/runtime.h>

static uint16_t readBOM(std::ifstream& ifs) {
    uint16_t BOM;
    ifs.read(reinterpret_cast<char*>(&BOM), sizeof(BOM));
    return BOM;
}

Runtime::Runtime() {
    // open ROM file and determine endianness
    appfile.open(this->appfile_name, std::ios::binary);
    uint16_t BOM = readBOM(appfile);
    srcendian = (readBOM(appfile) == 0xFEFF) ? Endian::Big : Endian::Little;
    sysendian = isSystemLittleEndian() ? Endian::Little : Endian::Big;

    // grab main memory
    this->main_memory = Memory::getInstance();
    std::cout << "memory instance loaded\n";
}

Runtime::~Runtime() {
    appfile.close();
}

void Runtime::start() {
    // Load application
    // execute
}

void Runtime::load_application() {}