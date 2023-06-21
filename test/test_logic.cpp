#include <iostream>
#include <gtest/gtest.h>

#include "checkerslogic.h"

TEST(CheckersLogicTests, WhiteTurnFirst)
{
    CheckersLogic logic;
    EXPECT_TRUE(logic.is_white_turn());
}

TEST(CheckersLogicTests, GetBetween)
{
    EXPECT_EQ(CheckersLogic::get_between(0x4000, 0x20000), 0);
    EXPECT_EQ(CheckersLogic::get_between(0x4000, 0x200000), 0x40000);
    EXPECT_EQ(CheckersLogic::get_between(0x10000000, 0x40000), 0);
    EXPECT_EQ(CheckersLogic::get_between(0x40000, 0x10000000), 0x2200000);
    EXPECT_EQ(CheckersLogic::get_between(0x1, 0x80000000), 0x08442210);
}

TEST(CheckersLogicTests, ManStepMoves)
{
    state_t s = 0x41610288;
    EXPECT_EQ(CheckersLogic::man_step_moves(s, true), 0x1e1038c0);
    EXPECT_EQ(CheckersLogic::man_step_moves(s, false), 0xc1e1038);
}

TEST(CheckersLogicTests, ManEatMoves)
{
    state_t s = 0x41610288;
    state_t b = 0x04041040;
    EXPECT_EQ(CheckersLogic::man_eat_moves(s, b), 0x60216600);
}

TEST(CheckersLogicTests, KingEatMovesUnfiltered)
{
    state_t s = 0x80200050;
    state_t b = 0x04061800;
    state_t p = 0x84261850;

    EXPECT_EQ(CheckersLogic::king_eat_moves_unfiltered(s, b, p), 0x4950e200);
}

TEST(CheckersLogicTests, KingEatMovesInDirection)
{
    state_t s = 0x50000004;
    state_t b = 0x00300240;
    state_t p = 0x50300244;

    const auto tr = Direction::top_right();
    const auto tl = Direction::top_left();
    const auto br = Direction::bottom_right();
    const auto bl = Direction::bottom_left();

    EXPECT_EQ(
        CheckersLogic::king_eat_moves_in_direction(s, b, p, tr),
        0x800);
    EXPECT_EQ(
        CheckersLogic::king_eat_moves_in_direction(s, b, p, tl),
        0x11000);
    EXPECT_EQ(
        CheckersLogic::king_eat_moves_in_direction(s, b, p, br),
        0x40000);
    EXPECT_EQ(
        CheckersLogic::king_eat_moves_in_direction(s, b, p, bl),
        0x21000);
}

TEST(CheckersLogicTests, KingEatMoves)
{
    state_t s = 0x80200041;
    state_t b = 0x04061800;
    state_t p = 0x84261841;

    EXPECT_EQ(CheckersLogic::king_eat_moves(s, b, p), 0x4150e200);
}

