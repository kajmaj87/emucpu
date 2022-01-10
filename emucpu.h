#ifndef __EMUCPU_H
#define __EMUCPU_H

#include <array>
#include <iostream>
#include <span>

enum Register : uint8_t { R0, R1, R2, R3, R4, R5, R6, R7, PC };

enum Opcode : uint8_t {
  LOAD = 0x00,
  MOVE = 0x01,
  STORE = 0x10,
  READ = 0x20,
  ADD = 0x30,
  SUB = 0x40,
  INC = 0x50,
  DEC = 0x51,
  SWAP = 0x90,
  JMP = 0xa0,
  JZ = 0xa1,
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

  constexpr uint8_t lookup(uint16_t index) {
    return static_cast<uint8_t>(mem[index]);
  }
  constexpr uint16_t lookup2b(uint16_t index) {
    return to_word(lookup(index), lookup(index + 1));
  }

  void process(const Instruction inst);
  void load_program(const std::span<Instruction> program);
  void execute(const std::span<Instruction> program);

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

  void print_debug_info() {
    print_registers();
    print_memory();
  }

  Emucpu() { mem.fill(std::byte{0xff}); };
};

#endif
