#include "emucpu.cpp"

const std::vector<Instruction> fibonacci{
    {LOAD, R0, 0x00, 0x08}, // n in fib(n)
    {LOAD, R1, 0x00, 0x00}, // f(0)
    {LOAD, R2, 0x00, 0x01}, // f(1)
    {ADD, R1, R1, R2},      {SWAP, R1, R2, NOOP},
    {DEC, R0, NOOP, NOOP},  {JNZ, R0, 0x00, 12}};

int main() {
  Emucpu emu;
  emu.load_program(fibonacci);
  emu.execute();
  emu.print_debug_info();
  return 0;
}
