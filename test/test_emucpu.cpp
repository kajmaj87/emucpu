#include <gtest/gtest.h>
#include <cstdio>

class EmucpuTest : public testing::Test {
protected:
    EmucpuTest() = default;
};

TEST_F(EmucpuTest, RunsTest) {
    EXPECT_TRUE(true);
}

int main(int ac, char *av[]) {
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
