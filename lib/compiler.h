#ifndef __EMUCPU_COMPILER_H
#define __EMUCPU_COMPILER_H

#include "emucpu.h"
#include <map>
#include <string>
#include <vector>

class Compiler {

private:
  const std::map<std::string, Opcode> opcodes_mapping = {
      {"LOAD", LOAD}, {"MOVE", MOVE}, {"ADD", ADD}, {"SUB", SUB}};
  const std::map<std::string, Register> registers_mapping = {
      {"R0", R0}, {"R1", R1}, {"R2", R2}, {"R3", R3}, {"R4", R4},
      {"R5", R5}, {"R6", R6}, {"R7", R7}, {"PC", PC}};

  Opcode to_opcode(std::string opcode) { return opcodes_mapping.at(opcode); }

  Register to_register(std::string reg) { return registers_mapping.at(reg); }

public:
  Instruction from_line(std::string line);

  std::vector<Instruction> compile(std::span<std::string> lines);
};

#endif
