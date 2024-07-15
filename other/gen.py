import re
import sys
from typing import List, Dict, Any
from itertools import combinations

def read_input_file(file_path: str) -> str:
    try:
        with open(file_path, 'r') as file:
            return file.read()
    except IOError as e:
        print(f"Error reading file: {e}")
        sys.exit(1)

# parse ssim-like tuple input
def parse_input(input_text: str) -> Dict[str, List[Dict[str, str]]]:
    tables = {}
    seen_lines = set()
    for line in input_text.split('\n'):
        line = line.strip()
        if not line or line in seen_lines:
            continue
        seen_lines.add(line)
        match = re.match(r'oni16\.(\w+)\s+(.+)', line)
        if match:
            table_name = match.group(1)
            key_values = match.group(2).split()
            entry = {}
            for kv in key_values:
                key, value = kv.split(':')
                entry[key] = value
            if table_name not in tables:
                tables[table_name] = []
            tables[table_name].append(entry)
    return tables

def generate_register_combinations(addressable_regs: List[str]) -> List[str]:
    combinations_code = []
    seen_combinations = set()
    for i in range(1, len(addressable_regs) + 1):
        if (i & (i - 1)) == 0:  # i == 2^x
            for combo in combinations(addressable_regs, i):
                sorted_combo = tuple(sorted(combo))
                if sorted_combo not in seen_combinations:
                    seen_combinations.add(sorted_combo)
                    reg_name = ''.join(sorted_combo)
                    type_name = f"uint{i * 16}_t"
                    combinations_code.append(f"    {type_name} {reg_name}() const;")
    return combinations_code

def generate_register_class(registers: List[Dict[str, str]]) -> str:
    class_code = []
    class_code.append("namespace oni16 {")
    class_code.append("class Registers {")
    
    for reg in registers:
        if reg['n_addressable'] == 'Y':
            class_code.append(f"    union {{")
            class_code.append(f"        uint16_t {reg['register']};")
            class_code.append(f"        struct {{")
            class_code.append(f"            uint8_t {reg['register']}L;")
            class_code.append(f"            uint8_t {reg['register']}H;")
            class_code.append(f"        }};")
            class_code.append(f"    }};")
        else:
            class_code.append(f"    uint16_t {reg['register']};")
    
    class_code.append("  public:")
    constructor_init = ", ".join(f"{r['register']}(0)" for r in registers)
    class_code.append(f"    Registers() : {constructor_init} {{}}")
    
    # register combinations
    addressable_regs = [r['register'] for r in registers if r['n_addressable'] == 'Y']
    combinations_code = generate_register_combinations(addressable_regs)
    class_code.extend(combinations_code)
    
    # Generate set and get methods
    class_code.append("    void set(REGISTERS::REGISTER, uint16_t value);")
    class_code.append("    uint16_t get(REGISTERS::REGISTER) const;")
    
    class_code.append("};")
    
    class_code.append("enum class REGISTER : unsigned char {")
    for i, reg in enumerate(registers):
        class_code.append(f"    {reg['register']} = {i},")
    class_code.append("    NUM_REGISTERS")
    class_code.append("};")
    
    class_code.append("#define Accessible(X) constexpr unsigned char X = static_cast<int>(REGISTER::X)")
    for reg in registers:
        class_code.append(f"Accessible({reg['register']});")
    class_code.append("Accessible(NUM_REGISTERS);")
    class_code.append("#undef Accessible")
    
    class_code.append("using REG = REGISTER;")
    class_code.append("} // namespace oni16") 
    
    return "\n".join(class_code)

def generate_code(tables: Dict[str, List[Dict[str, str]]]) -> str:
    generated_code = []
    
    if 'register' in tables:
        generated_code.append(generate_register_class(tables['register']))
    
    # more generators...
    
    return "\n\n".join(generated_code)

def main():
    if len(sys.argv) != 2:
        print("Usage: python gen.py <input_file>")
        sys.exit(1)

    input_file_path = sys.argv[1]
    input_text = read_input_file(input_file_path)
    tables = parse_input(input_text)
    generated_code = generate_code(tables)
    print(generated_code)

if __name__ == "__main__":
    main()