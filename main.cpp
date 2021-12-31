#include "emucpu.cpp"

const std::vector<Instruction> fibonacci{
    {LOAD, R0, 0x00, 0x01}, // n in fib(n)
    {LOAD, R1, 0x00, 0x00}, // f(0)
    {LOAD, R2, 0x00, 0x01}, // f(1)
    {JZ, R0, 0x00, 28},     {ADD, R1, R1, R2},  {SWAP, R1, R2, NOOP},
    {DEC, R0, NOOP, NOOP},  {JNZ, R0, 0x00, 16}};

int main() {
  Emucpu emu;
  emu.execute(fibonacci);
  emu.print_debug_info();
  return 0;
}
