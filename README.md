# Basic Opcodes

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

# Registers

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

# Reserved Keywords

- SECTION
- .DATA
- .RAM
- .VRAM
- .SWAP
- .ROM

# Shortcuts

- @: Current address (PC?)

## TODO

### Assembler:
- add keywords (section/segment access, etc)
- add some special operators (@, in-line arithmetic, ...)
- add some 'system calls' (printf, ...)

### Runtime:
- ...

