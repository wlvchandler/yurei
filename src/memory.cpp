#include <j16/memory.h>

Memory* Memory::instance_ = nullptr;

Memory::Memory() {
    static size_t n = 0;
    std::cout << n++ << " - " << __FUNCTION__ << std::endl;
    using namespace REGISTERS;
    address_space = new uint16_t[ADDRESS_SPACE_SIZE]();
    registers = new uint16_t[NUM_REGISTERS]();

    registers[SP] = 0x9fff;
    registers[PC] = 0x0; // update to start of program code segment.
}

Memory::~Memory() {
    delete[] address_space;
    delete[] registers;
}

Memory* Memory::getInstance() {
    if (instance_ == nullptr) {
        std::cout << "Creating new Memory instance\n";
        instance_ = new Memory;
        instance_->initializeSegments();
    }
    return instance_;
}

void Memory::initializeSegments() {
    logger::info << "Defining memory segments\n";
    logger::info << "DATA : 0x0000 - 0x1FFF\n";
    segments.push_back(Segment(0x0000, 0x1fff)); // DATA
    logger::info << "RAM : 0x2000 - 0x5FFF\n";
    segments.push_back(Segment(0x2000, 0x5fff)); // RAM
    logger::info << "VRAM : 0x6000 - 0x9FFF\n";
    segments.push_back(Segment(0x6000, 0x9fff)); // VRAM
    logger::info << "SWAP : 0xA000 - 0xDFFF\n";
    segments.push_back(Segment(0xA000, 0xdfff)); // SWAP
    logger::info << "ROM : 0xE000 - 0xFFFF\n";
    segments.push_back(Segment(0xE000, 0xffff)); // ROM
}

uint16_t Memory::get_pc() const {
    return this->registers[REGISTERS::PC];
}

// VBlank:  (triggered when display is done refreshing) - actions like updating the VRAM while the screen is not
// drawing. Timer:    triggered when the Timer Counter overflows. LCD STAT: triggered when there are changes in the LCD
// status. This could include mode changes like transitioning from VBlank to HBlank. Serial Transfer Completion
// Interrupt Joypad  : when there's a state change in the joypad buttons (pressed)
void Memory::check_for_interrupts() {
    using REGISTERS::PC;

    if (this->address_space[0x0040] & 0x01) { // VBlank
        // push_to_stack(this->registers[PC]);
        // this->registers[PC] = 0x0100;
    }

    if (this->address_space[0x0048] & 0x01) { // Timer
        // push_to_stack(this->registers[PC]);
        // this->registers[PC] = 0x0100;
    }

    if (this->address_space[0x0050] & 0x01) { // LCD
        // push_to_stack(this->registers[PC]);
        // this->registers[PC] = 0x0100;
    }

    if (this->address_space[0x0058] & 0x01) { // STC
        // push_to_stack(this->registers[PC]);
        // this->registers[PC] = 0x0100;
    }

    if (this->address_space[0x0060] & 0x01) { // STC
        // push_to_stack(this->registers[PC]);
        // this->registers[PC] = 0x0100;
    }
}

uint16_t Memory::execute_memory() {
    using namespace REGISTERS;

    std::uint16_t exit_code = 0;
    Opcode c_instruction;
    size_t DEBUG_instructions = 0;
    size_t DEBUG_max_instructions = 10;

    bool executing = true;
    while (executing && (DEBUG_instructions++ < DEBUG_max_instructions)) {
        check_for_interrupts();
        c_instruction.load(address_space[registers[PC]]);
        this->increment_pc();
    }

    if (exit_code != 0) {}
    dump(); // DEBUG

    return exit_code;
}

uint16_t* Memory::getMemory() {
    return address_space;
}

void Memory::increment_pc() {
    using REGISTERS::PC;

    if (registers[PC] < 65535) {
        this->registers[PC]++;

        std::cout << "PC now " << this->registers[PC] << "\n";
    } else {
        // handle
    }
}

Segment::Segment(uint16_t start, uint16_t end) : start(start), size(end - start + 1) {
    base = &(Memory::getInstance()->getMemory()[start]);
}

Segment::~Segment() {}

uint16_t Segment::read(uint16_t address) {
    if (address < size) {
        return base[address];
    } else {
        throw std::out_of_range("MemRead error: Address is out of range for this segment");
    }
}

void Segment::write(uint16_t address, uint16_t data) {
    if (address < size) {
        base[address] = data;
    } else {
        throw std::out_of_range("MemWrite error: Address is out of range for this segment");
    }
}

static void print_registers(uint16_t* registers) {
    using namespace REGISTERS;

    std::cout << std::hex;
    std::cout << "-----------------" << std::endl;
    std::cout << "Register Contents" << std::endl;
    std::cout << "-----------------" << std::endl;

    auto _print_register = [](const char* r, uint16_t v) {
        std::cout << r << "\t 0x" << std::setfill('0') << std::setw(2) << v << std::endl;
    };
    _print_register("A", registers[A]);
    _print_register("B", registers[B]);
    _print_register("C", registers[C]);
    _print_register("D", registers[D]);
    _print_register("SP", registers[SP]);
    _print_register("PC", registers[PC]);
    _print_register("FLAGS", registers[FLAGS]);
    std::cout << std::dec;
}

static void print_address_space(uint16_t* address_space, uint16_t sz = 65535) {
    std::cout << std::hex;
    std::cout << "-----------------" << std::endl;
    std::cout << "Memory" << std::endl;
    std::cout << "-----------------" << std::endl;
    unsigned cols = 16;
    unsigned rows = ((sz + 1) / cols);
    for (size_t i = 0; i < rows; i++) {
        std::cout << "0x" << std::setfill('0') << std::setw(2) << i * cols << "\t";
        std::string readable;
        for (size_t j = 0; j < cols; j++) {
            uint16_t value = address_space[i * cols + j];
            std::cout << std::setfill('0') << std::setw(2) << value << " ";
            readable += (value > ' ' && value < 128 ? static_cast<char>(value) : '.');
        }
        std::cout << "  |" << readable << "|" << std::endl;
    }
    std::cout << std::dec;
}

void Memory::dump() {
    std::cout << "---------------------------" << std::endl;
    std::cout << "--------MEMORY DUMP--------" << std::endl;
    std::cout << "---------------------------" << std::endl;
    print_registers(this->registers);
    std::cout << std::endl;
    print_address_space(this->address_space, 256);
    // print_address_space(this->address, this->ADDRESS_SPACE_SIZE);
}

// 32-bit pair-addressable register combinations
inline uint32_t Registers::AB() const {
    return (static_cast<uint32_t>(A) << 16) | B;
}
inline uint32_t Registers::AC() const {
    return (static_cast<uint32_t>(A) << 16) | A;
}
inline uint32_t Registers::AD() const {
    return (static_cast<uint32_t>(A) << 16) | D;
}
inline uint32_t Registers::BC() const {
    return (static_cast<uint32_t>(B) << 16) | C;
}
inline uint32_t Registers::BD() const {
    return (static_cast<uint32_t>(B) << 16) | D;
}
inline uint32_t Registers::CD() const {
    return (static_cast<uint32_t>(C) << 16) | D;
}

// 64-bit quad-addressable register
inline uint64_t Registers::ABCD() const {
    return (static_cast<uint64_t>(A) << 48) | (static_cast<uint64_t>(B) << 32) | (static_cast<uint64_t>(C) << 16) | D;
}

inline void Registers::set(REGISTERS::REGISTER reg, uint16_t value) {
    using namespace REGISTERS;
    switch (reg) {
    case REGISTER::A:
        this->A = value;
        break;
    case REGISTER::B:
        this->B = value;
        break;
    case REGISTER::C:
        this->C = value;
        break;
    case REGISTER::D:
        this->D = value;
        break;
    case REGISTER::SP:
        this->SP = value;
        break;
    case REGISTER::PC:
        this->PC = value;
        break;
    case REGISTER::FLAGS:
        this->FLAGS = value;
        break; 
    default:
        break;
    };
}

inline uint16_t Registers::get(REGISTERS::REGISTER reg) const {
    using namespace REGISTERS;
    uint16_t r = 0;
    switch (reg) {
    case REGISTER::A:
        r = this->A;
        break;
    case REGISTER::B:
        r = this->B;
        break;
    case REGISTER::C:
        r = this->C;
        break;
    case REGISTER::D:
        r = this->D;
        break;
    case REGISTER::SP:
        r = this->SP;
        break;
    case REGISTER::PC:
        r = this->PC;
        break;
    case REGISTER::FLAGS:
        r = this->FLAGS;
        break;
    default:
        r = 0;
    }
    return r;
}