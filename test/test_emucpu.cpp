#include "emucpu.cpp"
#include <cstdio>
#include <gtest/gtest.h>

TEST(LOAD_REGISTERS, RunsTest) {
  for (uint8_t i = 0; i < 8; i++) {
    Emucpu e;
    Instruction p{LOAD, i, 0x00, 0x01};
    e.process(p);
    ASSERT_EQ(e.reg[i], 1);
  }
}

TEST(LOAD_BIG_NUMBERS, RunsTest) {
  Emucpu e;
  Instruction p{LOAD, 0x00, 0xff, 0xff};
  e.process(p);
  Instruction r{LOAD, 0x01, 0x0f, 0x00};
  e.process(r);
  ASSERT_EQ(e.reg[0], 0xffff);
  ASSERT_EQ(e.reg[1], 0x0f00);
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}

