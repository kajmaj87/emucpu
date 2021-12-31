#include <array>
#include <cstddef>
#include <iostream>
#include <vector>

enum Register : uint8_t { R0, R1, R2, R3, R4, R5, R6, R7, PC };

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

struct Instruction {
  Opcode op;
  uint8_t v1;
  uint8_t v2;
  uint8_t v3;
};

class Emucpu {

  constexpr static uint16_t TOTAL_MEMORY{0x20};
  constexpr static uint16_t MAX_ADDRESS{
      static_cast<uint16_t>(TOTAL_MEMORY - 1)};
  constexpr static uint16_t REGISTERS{9};

public:
  uint16_t reg[REGISTERS] = {0};
  std::array<std::byte, TOTAL_MEMORY> mem;

  constexpr uint16_t to_word(const uint8_t upper, const uint8_t lower) {
    return (upper << 8) | lower;
  };

  void process(const Instruction inst) {
    switch (inst.op) {
    case LOAD: {
      uint8_t register_number = inst.v1;
      uint16_t value = to_word(inst.v2, inst.v3);
      reg[register_number] = value;
      std::printf("Loaded %d to r%d\n", value, register_number);
      break;
    }
    case STORE: {
      uint8_t register_number = inst.v1;
      uint16_t memory_address = to_word(inst.v2, inst.v3);
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
    case ADD: {
      uint8_t result = inst.v1;
      uint8_t a = inst.v2;
      uint8_t b = inst.v3;
      auto old_a = reg[a];
      auto old_b = reg[b];
      reg[result] = reg[a] + reg[b];
      std::printf(
          "Added %d (0x%04x) to %d (0x%04x)  and saved in R%d -> %d 0x%04x\n",
          old_a, old_a, old_b, old_b, result, reg[result], reg[result]);
      break;
    }
    case SWAP: {
      uint8_t a = inst.v1;
      uint8_t b = inst.v2;
      uint16_t temp = reg[b];
      reg[b] = reg[a];
      reg[a] = temp;
      std::printf("Swapped R%d (%d 0x%04x) with R%d (%d 0x%04x)\n", a, reg[a],
                  reg[a], b, reg[b], reg[b]);
      break;
    }
    case DEC: {
      uint8_t a = inst.v1;
      reg[a] = reg[a] - 1;
      std::printf("Decreased R%d to (%d 0x%04x)\n", a, reg[a], reg[a]);
      break;
    }
    case JNZ: {
      uint8_t a = inst.v1;
      if (reg[a] != 0) {
        if (inst.v2 > 0) {
          std::printf("WARNING! JEZ not implemented for adresses above 256!\n");
        }
        uint8_t b = inst.v3;
        std::printf("Jumping from %d (0x%04x) to %d (0x%04x)\n", reg[PC],
                    reg[PC], b, b);
        reg[PC] =
            b - 4; // adjust for program counter being moved after each inst
      }
      break;
    }
    case UNINITIALIZED: {
      std::printf("Uninitialized memory access!\n");
      break;
    }
    }
    reg[PC] += 4;
  };
  void load_program(const std::vector<Instruction> program) {
    for (int i = 0; i < std::size(program); i++) {
      mem[4 * i] = std::byte{program[i].op};
      mem[4 * i + 1] = std::byte{program[i].v1};
      mem[4 * i + 2] = std::byte{program[i].v2};
      mem[4 * i + 3] = std::byte{program[i].v3};
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
  void execute() {
    while (reg[PC] < MAX_ADDRESS) {
      struct Instruction i;
      i.op = static_cast<Opcode>(mem[reg[PC]]);
      i.v1 = static_cast<uint8_t>(mem[reg[PC] + 1]);
      i.v2 = static_cast<uint8_t>(mem[reg[PC] + 2]);
      i.v3 = static_cast<uint8_t>(mem[reg[PC] + 3]);
      process(i);
    }
    if (reg[PC] > MAX_ADDRESS) {
      std::printf("Out of memory access!\n");
    }
  }
  void print_debug_info() {
    print_registers();
    print_memory();
  }
  uint8_t lookup(uint16_t index) { return static_cast<uint8_t>(mem[index]); }
  uint16_t lookup2b(uint16_t index) {
    return to_word(lookup(index), lookup(index + 1));
  }

  Emucpu() { mem.fill(std::byte{0xff}); };
};
