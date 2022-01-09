#include "emucpu.h"
#include "lib/compiler.h"
#include "lib/parser.h"
#include <cstdio>
#include <gtest/gtest.h>

class CompilerTest : public testing::Test {
protected:
  CompilerTest() = default;
  Emucpu e;
  Compiler c;
  Parser p;
};

uint16_t compile_and_execute(std::string line, Compiler c, Parser p, Emucpu e) {
  std::vector<std::string> lines = {line};
  auto assembly = p.parse(lines);
  auto bytecode = c.compile(assembly);
  e.execute(bytecode);
  return e.reg[0];
}

/* TEST_F(CompilerTest, simple_addition_should_work) { */
/*   ASSERT_EQ(4, compile_and_execute("2+2", c, p, e)); */
/* } */

/* TEST_F(CompilerTest, multiple_additions) { */
/*   ASSERT_EQ(10, compile_and_execute("1+2+3+4", c, p, e)); */
/* } */

/* TEST_F(CompilerTest, simple_subtraction) { */
/*   ASSERT_EQ(3, compile_and_execute("5-2", c, p, e)); */
/* } */

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
