#include <iostream>
#include <cstddef>
#include <array>

constexpr uint16_t MAX_ADDRESS { 0x10 };
constexpr uint16_t REGISTERS { 8 };

enum Opcode { LOAD = 0x00, STORE = 0x10, UNINITIALIZED = 0xff };

std::array<std::byte, MAX_ADDRESS> mem;
uint16_t reg[REGISTERS] = {0};

int pc = 0;

void initialize_memory(){
  mem.fill(std::byte{0xff});
};

void load_program(){
  mem[0] = std::byte {LOAD};
  mem[1] = std::byte { 0x01 };
  mem[2] = std::byte { 0x00 };
  mem[3] = std::byte { 0xa0 };
  mem[4] = std::byte {LOAD};
  mem[5] = std::byte { 0x02 };
  mem[6] = std::byte { 0x01 };
  mem[7] = std::byte { 0x00 };
  mem[8] = std::byte {STORE};
  mem[9] = std::byte { 0x01 };
  mem[10] = std::byte { 0x00 };
  mem[11] = std::byte { 0x0f };
};

constexpr uint16_t to_word(const std::byte upper, const std::byte lower) {
  return (std::to_integer<uint8_t>(upper) << 8) | std::to_integer<uint8_t>(lower);
};

void process(const std::byte opcode, const std::byte v1, const std::byte v2, const std::byte v3){
  if (opcode == std::byte{ LOAD }) {
     uint8_t register_number = std::to_integer<uint8_t>(v1);
     uint16_t value = to_word(v2, v3);
     reg[register_number] = value;
     std::printf("Loaded %d to r%d\n", value, register_number);
  }
  if (opcode == std::byte{ STORE }) {
     uint8_t register_number = std::to_integer<uint8_t>(v1);
     uint16_t memory_address = to_word(v2, v3);
     mem[memory_address] = std::byte(reg[register_number]);
     std::printf("Stored %d (%02x) under memory address %04x\n", reg[register_number], reg[register_number], memory_address);
  }
  if (opcode == std::byte{ UNINITIALIZED }) {
     std::printf("Uninitialized memory access!\n");
  }
};

void print_registers() {
  for (int i=0; i<8; i++){
    std::printf("r%d: %4d (0x%04x)\n", i, reg[i], reg[i]);
  }
};

void print_memory() {
  for (int i=0; i<MAX_ADDRESS; i++){
    if (i % 4 == 0) {
      std::printf("%04x-%04x: ", i, i + 3);
    }
    std::printf("%02x ", unsigned(mem[i]));
    if (i % 4 == 3) {
      std::printf("\n");
    }
  }
};

int main() {
    initialize_memory();
    load_program();
    while(pc < MAX_ADDRESS) {
      process(mem[pc], mem[pc+1], mem[pc+2], mem[pc+3]); 
      pc+=4;
    }
    if (pc > MAX_ADDRESS) {
      std::printf("Out of memory access!");
    }
    print_registers();
    print_memory();
    return 0;
}

