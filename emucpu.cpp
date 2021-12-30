#include <array>
#include <cstddef>
#include <iostream>

constexpr uint16_t TOTAL_MEMORY{0x20};
constexpr uint16_t MAX_ADDRESS{TOTAL_MEMORY - 1};
constexpr uint16_t REGISTERS{8};

std::array<std::byte, TOTAL_MEMORY> mem;
uint16_t reg[REGISTERS] = {0};

int pc = 0;

void initialize_memory() { mem.fill(std::byte{0xff}); };

enum Opcode : uint8_t {
  LOAD = 0x00,
  STORE = 0x10,
  READ = 0x20,
  ADD = 0x30,
  INC = 0x50,
  DEC = 0x51,
  SWAP = 0x90,
  JMP = 0xa0,
  JEZ = 0xa1,
  JNZ = 0xa2,
  NOOP = 0xfe,
  UNINITIALIZED = 0xff
};
enum Register : uint8_t { R0, R1, R2, R3, R4, R5, R6, R7, PC };

void load_program() {
  constexpr uint8_t program[] = {LOAD, R0, 0x00, 0x07, // n in fib(n)
                                 LOAD, R1, 0x00, 0x00, // f(0)
                                 LOAD, R2, 0x00, 0x01, // f(1)
                                 ADD,  R1, R1,   R2,   SWAP, R1, R2,   NOOP,
                                 DEC,  R0, NOOP, NOOP, JNZ,  R0, 0x00, 8};

  for (int i = 0; i < std::size(program); i++) {
    mem[i] = std::byte{program[i]};
  }
};

constexpr uint16_t to_word(const std::byte upper, const std::byte lower) {
  return (std::to_integer<uint8_t>(upper) << 8) |
         std::to_integer<uint8_t>(lower);
};

void process(const std::byte opcode, const std::byte v1, const std::byte v2,
             const std::byte v3) {
  switch (opcode) {
  case std::byte{LOAD}: {
    uint8_t register_number = std::to_integer<uint8_t>(v1);
    uint16_t value = to_word(v2, v3);
    reg[register_number] = value;
    std::printf("Loaded %d to r%d\n", value, register_number);
    break;
  }
  case std::byte{STORE}: {
    uint8_t register_number = std::to_integer<uint8_t>(v1);
    uint16_t memory_address = to_word(v2, v3);
    if (memory_address < MAX_ADDRESS) {
      mem[memory_address] = std::byte(reg[register_number] >> 8);
      mem[memory_address + 1] = std::byte(reg[register_number]);
      std::printf("Stored %d (%02x) under memory address %04x-%04x\n",
                  reg[register_number], reg[register_number], memory_address,
                  memory_address + 1);
    } else {
      std::printf("Attempted to store %d (%02x) under address %04x-%04x but "
                  "max adress is %04x\n",
                  reg[register_number], reg[register_number], memory_address,
                  memory_address + 1, MAX_ADDRESS);
    }
    break;
  }
  case std::byte{ADD}: {
    uint8_t result = std::to_integer<uint8_t>(v1);
    uint8_t a = std::to_integer<uint8_t>(v2);
    uint8_t b = std::to_integer<uint8_t>(v3);
    auto old_a = reg[a];
    auto old_b = reg[b];
    reg[result] = reg[a] + reg[b];
    std::printf(
        "Added %d (0x%04x) to %d (0x%04x)  and saved in R%d -> %d 0x%04x\n",
        old_a, old_a, old_b, old_b, result, reg[result], reg[result]);
    break;
  }
  case std::byte{SWAP}: {
    uint8_t a = std::to_integer<uint8_t>(v1);
    uint8_t b = std::to_integer<uint8_t>(v2);
    uint16_t temp = reg[b];
    reg[b] = reg[a];
    reg[a] = temp;
    std::printf("Swapped R%d (%d 0x%04x) with R%d (%d 0x%04x)\n", a, reg[a],
                reg[a], b, reg[b], reg[b]);
    break;
  }
  case std::byte{DEC}: {
    uint8_t a = std::to_integer<uint8_t>(v1);
    reg[a] = reg[a] - 1;
    std::printf("Decreased R%d to (%d 0x%04x)\n", a, reg[a], reg[a]);
    break;
  }
  case std::byte{JNZ}: {
    uint8_t a = std::to_integer<uint8_t>(v1);
    if (reg[a] != 0) {
      if (std::to_integer<uint8_t>(v2) > 0) {
        std::printf("WARNING! JEZ not implemented for adresses above 256!\n");
      }
      uint8_t b = std::to_integer<uint8_t>(v3);
      std::printf("Jumping from %d (0x%04x) to %d (0x%04x)\n", pc, pc, b, b);
      pc = b;
    }
    break;
  }
  case std::byte{UNINITIALIZED}: {
    std::printf("Uninitialized memory access!\n");
    break;
  }
  }
};

void print_registers() {
  for (int i = 0; i < 8; i++) {
    std::printf("r%d: %4d (0x%04x)\n", i, reg[i], reg[i]);
  }
};

void print_memory() {
  for (int i = 0; i < TOTAL_MEMORY; i++) {
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
  while (pc < MAX_ADDRESS) {
    process(mem[pc], mem[pc + 1], mem[pc + 2], mem[pc + 3]);
    pc += 4;
  }
  if (pc > MAX_ADDRESS) {
    std::printf("Out of memory access!\n");
  }
  print_registers();
  print_memory();
  return 0;
}
