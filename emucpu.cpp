#include <iostream>
#include <cstddef>
#include <array>

constexpr uint16_t MAX_ADDRESS { 0x0f };
constexpr uint16_t REGISTERS { 8 };

std::array<std::byte, MAX_ADDRESS> mem;
uint16_t reg[REGISTERS] = {0};

int pc = 0;

void initialize_memory(){
  mem.fill(std::byte{0xff});
}

void load_program(){
  mem[0] = std::byte { 0x00 };
  mem[1] = std::byte { 0x01 };
  mem[2] = std::byte { 0x00 };
  mem[3] = std::byte { 0xa0 };
  mem[4] = std::byte { 0x00 };
  mem[5] = std::byte { 0x02 };
  mem[6] = std::byte { 0x12 };
  mem[7] = std::byte { 0x0f };
}

uint16_t to_word(std::byte upper, std::byte lower){
  return (std::to_integer<uint8_t>(upper) << 8) | std::to_integer<uint8_t>(lower);
}

void process(std::byte opcode, std::byte v1, std::byte v2, std::byte v3){
  if (opcode == std::byte{0x00}) {
     uint8_t register_number = std::to_integer<uint8_t>(v1);
     uint16_t value = to_word(v2, v3);
     reg[register_number] = value;
     std::cout << "Loaded " << value << " to register " << register_number << "\n";
  }
  if (opcode == std::byte{0xff}) {
     std::cout << "Uninitialized memory access!\n";
  }
}

int main() {
    initialize_memory();
    load_program();
    while(pc < MAX_ADDRESS) {
      process(mem[pc], mem[pc+1], mem[pc+2], mem[pc+3]); 
      pc+=4;
    }
    if (pc > MAX_ADDRESS) {
      std::cout << "Out of memory access!";
    }
    return 0;
}

