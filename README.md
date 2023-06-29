## Build
### Dependencies
#### SDL (only required for screen output)
##### Linux
```
## apt
sudo apt update
sudo apt install libsdl2-dev

## dnf/yum
sudo dnf install SDL2-devel
sudo yum install SDL2-devel

## pacman
sudo pacman -S sdl2

## zypper
sudo zypper install libSDL2-devel

# Alternatively, from source:
## dependencies depend on distro, here's an idea of what you might need:
sudo apt-get install build-essential
sudo dnf/yum groupinstall "Development Tools"
sudo pacman -S base-devel
## you may also need: libfontconfig1-dev libfreetype6-dev libgl1-mesa-dev libglew-dev libglu1-mesa-dev


wget https://github.com/libsdl-org/SDL/releases/download/release-2.28.0/SDL2-2.28.0.tar.gz
tar -xzf SDL2-2.28.0.tar.gz
cd SDL2-2.28.0
./configure
make
sudo make install # for global install
```


##### Mac
Install Homebrew if you don't have it:
`/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
Then install
`brew install sdl2`


## Documentation
### Basic Opcodes

| Mnemonic | Operands     | Opcode | Operand1 | Operand2 |
|----------|--------------|--------|----------|----------|
| NOP      | -            | 0x0000 | -        | -        |
| HALT     | -            | 0x0001 | -        | -        |
| LOAD     | R, IMM       | 0x000A | REG      | IMM/ADDR |
| STORE    | R, ADDR      | 0x000B | REG      | ADDR     |
| MOV      | R1, R2       | 0x000C | REG      | REG      |
| PUSH     | R            | 0x000D | REG      | -        |
| POP      | R            | 0x000E | REG      | -        |
| ADD      | R1, R2       | 0x0010 | REG      | REG      |
| SUB      | R1, R2       | 0x0011 | REG      | REG      |
| MUL      | R1, R2       | 0x0012 | REG      | REG      |
| SMUL     | R1, R2       | 0x0013 | -        | -        |
| DIV      | R1, R2       | 0x0014 | REG      | REG      |
| SDIV     | R1, R2       | 0x0015 | REG      | REG      |
| AND      | R1, R2       | 0x0020 | REG      | REG      |
| OR       | R1, R2       | 0x0021 | REG      | REG      |
| XOR      | R1, R2       | 0x0022 | REG      | REG      |
| NOT      | R            | 0x0023 | REG      | -        |
| SHR      | R            | 0x0024 | REG      | -        |
| SHL      | R            | 0x0025 | REG      | -        |
| JUMP     | ADDR         | 0x0030 | ADDR     | -        |
| JZ       | ADDR         | 0x0031 | ADDR     | -        |
| JNZ      | ADDR         | 0x0032 | ADDR     | -        |
| JC       | ADDR         | 0x0033 | ADDR     | -        |
| JNC      | ADDR         | 0x0034 | ADDR     | -        |
| CALL     | ADDR         | 0x0040 | ADDR     | -        |
| RET      | -            | 0x0041 | -        | -        |

### Registers

| Register | Opcode | Operand1 | Operand2 |
|----------|--------|----------|----------|
| A        | 0x00F0 | -        | -        |
| B        | 0x00F1 | -        | -        |
| C        | 0x00F2 | -        | -        |
| D        | 0x00F3 | -        | -        |
| I        | 0x00F4 | -        | -        |
| J        | 0x00F5 | -        | -        |
| SP       | 0x00F6 | -        | -        |
| PC       | 0x00F7 | -        | -        |
| FLAGS    | 0x00F8 | -        | -        |

### Reserved Keywords (planned/todo)

- SECTION
- .DATA
- .RAM
- .VRAM
- .SWAP
- .ROM

### Shortcuts

- @: Current address (PC?)

## TODO

### Assembler:
- add keywords (section/segment access, etc)
- add some special operators (@, in-line arithmetic, ...)
- add some 'system calls' (printf, ...)

### Runtime:
- ...

### Screen
- TFT LCD screen (240x240 or 240x160)
- emulate SPI communication (simulate SPI data transfer)
- draw graphics in frame buffer then block image transfer to vscreen
- 60hz refresh
- color depth/palette modes
