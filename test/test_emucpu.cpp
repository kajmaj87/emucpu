#include "emucpu.cpp"
#include <cstdio>
#include <gtest/gtest.h>

class EmucpuTest : public testing::Test {
protected:
  EmucpuTest() = default;
  Emucpu e;
};

TEST_F(EmucpuTest, load_to_all_registers) {
  for (uint8_t i = 0; i < 8; i++) {
    Emucpu e;
    e.process({LOAD, i, 0x00, 0x01});
    ASSERT_EQ(e.reg[i], 0x01);
  }
}

TEST_F(EmucpuTest, load_some_big_numbers) {
  e.process({LOAD, R0, 0xff, 0xff});
  e.process({LOAD, R1, 0x0f, 0x00});
  ASSERT_EQ(e.reg[R0], 0xffff);
  ASSERT_EQ(e.reg[R1], 0x0f00);
}

TEST_F(EmucpuTest, store_small_number) {
  e.reg[R0] = 0xab;
  e.process({STORE, R0, 0x00, 0x08});
  ASSERT_EQ(e.lookup(0x08), 0x00);
  ASSERT_EQ(e.lookup(0x09), 0xab);
  ASSERT_EQ(e.lookup2b(0x08), 0x00ab);
}

TEST_F(EmucpuTest, store_big_number) {
  e.reg[R0] = 0xabcd;
  e.process({STORE, R0, 0x00, 0x08});
  ASSERT_EQ(e.lookup(0x08), 0xab);
  ASSERT_EQ(e.lookup(0x09), 0xcd);
  ASSERT_EQ(e.lookup2b(0x08), 0xabcd);
}

TEST_F(EmucpuTest, add_registers_and_store_in_third) {
  e.reg[R0] = 0x1010;
  e.reg[R1] = 0x0112;
  e.process({ADD, R2, R0, R1});
  ASSERT_EQ(e.reg[R2], 0x1122);
}

TEST_F(EmucpuTest, add_registers_and_store_in_one_of_them) {
  e.reg[R0] = 0x1010;
  e.reg[R1] = 0x0112;
  e.process({ADD, R0, R0, R1});
  ASSERT_EQ(e.reg[R0], 0x1122);
}

TEST_F(EmucpuTest, swap_registers) {
  e.reg[R0] = 0x1010;
  e.reg[R1] = 0x0112;
  e.process({SWAP, R0, R1, NOOP});
  ASSERT_EQ(e.reg[R0], 0x0112);
  ASSERT_EQ(e.reg[R1], 0x1010);
}

TEST_F(EmucpuTest, dec_register) {
  e.reg[R0] = 0x1010;
  e.process({DEC, R0, NOOP, NOOP});
  ASSERT_EQ(e.reg[R0], 0x100f);
}

TEST_F(EmucpuTest, jz_should_not_jump_when_register_is_not_zero) {
  e.reg[R0] = 0x1;
  e.process({JZ, R0, 0x00, 0x20});
  ASSERT_EQ(e.reg[PC], 0x04);
}

TEST_F(EmucpuTest, jz_should_jump_when_register_is_zero_to_small_address) {
  e.reg[R0] = 0x0;
  e.process({JZ, R0, 0x00, 0x20});
  ASSERT_EQ(e.reg[PC], 0x0020);
}

TEST_F(EmucpuTest, jnz_should_not_jump_when_register_is_zero) {
  e.reg[R0] = 0x0;
  e.process({JNZ, R0, 0x10, 0x20});
  ASSERT_EQ(e.reg[PC], 0x04);
}

TEST_F(EmucpuTest, jnz_should_jump_when_register_is_not_zero_to_small_address) {
  e.reg[R0] = 0x1;
  e.process({JNZ, R0, 0x00, 0x20});
  ASSERT_EQ(e.reg[PC], 0x0020);
}

TEST_F(EmucpuTest, should_calculate_fibonnaci_correctly) {
  int fib[] = {0, 1, 1, 2, 3, 5, 8, 13, 21};
  for (uint8_t i = 0; i <= 8; i++) {
    Emucpu e;
    const std::vector<Instruction> fibonacci{
        {LOAD, R0, 0x00, i},    // n in fib(n)
        {LOAD, R1, 0x00, 0x00}, // f(0)
        {LOAD, R2, 0x00, 0x01}, // f(1)
        {JZ, R0, 0x00, 28},     {ADD, R1, R1, R2},  {SWAP, R1, R2, NOOP},
        {DEC, R0, NOOP, NOOP},  {JNZ, R0, 0x00, 16}};
    e.execute(fibonacci);
    ASSERT_EQ(e.reg[R1], fib[i]) << "Fib[" << i << "] is " << fib[i];
  }
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
