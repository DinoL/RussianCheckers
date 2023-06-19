#include <iostream>
#include <gtest/gtest.h>

#include "bit_algo.h"

TEST(BitAlgoTests, ToState) {
    EXPECT_EQ(alg::to_state(5), 0b100000);
    EXPECT_EQ(alg::to_state(11), 0b100000000000);
    EXPECT_EQ(alg::to_state(31), 0b10000000000000000000000000000000);
}
