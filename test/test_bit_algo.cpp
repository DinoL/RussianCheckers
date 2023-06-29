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

TEST(BitAlgoTests, Add)
{
    state_t s = 0b10'001'000'010;
    state_t r = 0b10'100'000'000'001'010;
    EXPECT_EQ(alg::add(s, r), 0b10'100'010'001'001'010);
}

TEST(BitAlgoTests, Subtract)
{
    state_t s = 0b10'100'000'000'001'010;
    state_t r = 0b100'010'001'000'010;
    EXPECT_EQ(alg::subtract(s, r), 0b10'000'000'000'001'000);
}

TEST(BitAlgoTests, MovePiece)
{
    state_t s = 0b10'100'000'000'001'010;
    EXPECT_EQ(alg::move_piece(s, 2, 6), s);
    EXPECT_EQ(alg::move_piece(s, 3, 6), 0b10'100'000'001'000'010);
}

TEST(BitAlgoTests, FirstSetCell)
{
    EXPECT_EQ(alg::first_set_cell(0), -1);
    EXPECT_EQ(alg::first_set_cell(1), 0);
    EXPECT_EQ(alg::first_set_cell(0xc), 2);
    EXPECT_EQ(alg::first_set_cell(0xd1ac4000), 14);
    EXPECT_EQ(alg::first_set_cell(0x04020000), 17);
    EXPECT_EQ(alg::first_set_cell(0xff000000), 24);
    EXPECT_EQ(alg::first_set_cell(0x80000000), 31);
}

TEST(BitAlgoTests, Count)
{
    EXPECT_EQ(alg::count(0), 0);
    EXPECT_EQ(alg::count(1), 1);
    EXPECT_EQ(alg::count(0x800200), 2);
    EXPECT_EQ(alg::count(0x10000000), 1);
    EXPECT_EQ(alg::count(0xd1ac4000), 9);
}
