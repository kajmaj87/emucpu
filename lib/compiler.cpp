#include "lib/compiler.h"
#include <map>
#include <span>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string str, char delimiter){
  printf("Splitting: %s on %c\n", str.c_str(), delimiter);
  std::vector<std::string> array;
  std::stringstream ss(str);
  std::string tmp;
  while(std::getline(ss, tmp, delimiter))
  {
    array.push_back(tmp);
  }
  return array;
}

const uint8_t hi(const std::string &number) {
  return static_cast<uint8_t>(std::stoi(number) >> 8);
}

const uint8_t low(const std::string &number) {
  return static_cast<uint8_t>(std::stoi(number));
}

Instruction Compiler::from_line(std::string line) {
  line.erase(std::remove(line.begin(), line.end(), '\n'),
             line.end()); // remove endline
  std::vector tokens = split(line, ' ');
  printf("Looking for opcode: '%s'\n", tokens[0].c_str());
  Opcode opcode = to_opcode(tokens[0]);
  switch (opcode) {
    case LOAD:
      return {LOAD, to_register(tokens[1]), hi(tokens[2]), low(tokens[2])};
    case MOVE:
      return {MOVE, to_register(tokens[1]), to_register(tokens[2])};
    case ADD:
      return {ADD, to_register(tokens[1]), to_register(tokens[2]), to_register(tokens[3])};
    case SUB:
      return {SUB, to_register(tokens[1]), to_register(tokens[2]), to_register(tokens[3])};
    default:
      printf("Unrecognized instruction: %s", line.c_str());
      return {NOOP, NOOP, NOOP, NOOP};
  }
}

std::vector<Instruction> Compiler::compile(std::span<std::string> lines) {
  std::vector<Instruction> result;
  for (auto line: lines){
    result.push_back(from_line(line));
  }
  return result;
}
