#include <iostream>
#include <gtest/gtest.h>

#include "bit_algo.h"

TEST(BitAlgoTests, ToState) {
    EXPECT_EQ(alg::to_state(5), 0b100'000);
    EXPECT_EQ(alg::to_state(11), 0b100'000'000'000);
    EXPECT_EQ(alg::to_state(31), 0b10'000'000'000'000'000'000'000'000'000'000);
}

TEST(BitAlgoTests, SetPiece)
{
    state_t s = 0;
    EXPECT_EQ(alg::set_piece(s, 4), 0b10000);

    s = 0b10000;
    EXPECT_EQ(alg::set_piece(s, 4), s);

    s = 0b100'000'000'000;
    EXPECT_EQ(alg::set_piece(s, 2), 0b100'000'000'100);
    EXPECT_EQ(alg::set_piece(s, 31), 0b10'000'000'000'000'000'000'100'000'000'000);
}

TEST(BitAlgoTests, HasPiece)
{
    state_t s = 0b10'000'000'000'000'000'000'100'000'000'000;
    EXPECT_TRUE(alg::has_piece(s, 11));
    EXPECT_TRUE(alg::has_piece(s, 31));
    EXPECT_FALSE(alg::has_piece(s, 10));
}

TEST(BitAlgoTests, RemovePiece)
{
    state_t s = 0b10'000'000'000'000'000'000'100'000'000'000;
    EXPECT_EQ(alg::remove_piece(s, 31), 0b100'000'000'000);
    EXPECT_EQ(alg::remove_piece(s, 21), s);
}

TEST(BitAlgoTests, FirstSetPiece)
{
    state_t s = 0b10'000'000'000'000'000'000'100'000'000'000;
    EXPECT_EQ(alg::first_set_piece(s), 0b100'000'000'000);
    EXPECT_EQ(alg::first_set_piece(0), 0);
}

