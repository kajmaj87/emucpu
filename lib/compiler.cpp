#include "lib/compiler.h"
#include <map>
#include <span>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string str, char delimiter){
  std::vector<std::string> array;
  std::stringstream ss(str);
  std::string tmp;
  while(std::getline(ss, tmp, delimiter))
  {
    array.push_back(tmp);
  }
  return array;
}

uint8_t hi(std::string number){
  printf("Hi of %s is %i", number.c_str(), stoi(number) << 8);
  return static_cast<uint8_t> (stoi(number) << 8);
}

uint8_t low(std::string number){
  printf("Low of %s is %i", number.c_str(), stoi(number));
  return static_cast<uint8_t> (stoi(number));
}

Instruction Compiler::from_line(std::string line) {
  std::vector tokens = split(line, ' ');
  Opcode opcode = to_opcode(tokens[0]);
  switch (opcode) {
    case LOAD:
      return {LOAD, to_register(tokens[1]), hi(tokens[2]), low(tokens[2])};
    case ADD:
      return {ADD, to_register(tokens[1]), to_register(tokens[2]), to_register(tokens[3])};
    case SUB:
      return {SUB, to_register(tokens[1]), to_register(tokens[2]), to_register(tokens[3])};
  }
}

std::vector<Instruction> Compiler::compile(std::span<std::string> lines) {
  std::vector<Instruction> result;
  for (auto line: lines){
    result.push_back(from_line(line));
  }
  return result;
}
