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

TEST(CheckersLogicTests, StraightMovesInDirection)
{
    state_t s = 0x4c000340;
    const auto tr = Direction::top_right();
    const auto tl = Direction::top_left();
    const auto br = Direction::bottom_right();
    const auto bl = Direction::bottom_left();

    state_t tr_moves = CheckersLogic::straight_moves_in_direction(s, tr);
    EXPECT_EQ(tr_moves, 0xcc66b800);

    state_t tl_moves = CheckersLogic::straight_moves_in_direction(s, tl);
    EXPECT_EQ(tl_moves, 0x61133400);

    state_t br_moves = CheckersLogic::straight_moves_in_direction(s, br);
    EXPECT_EQ(br_moves, 0x08c8803e);

    state_t bl_moves = CheckersLogic::straight_moves_in_direction(s, bl);
    EXPECT_EQ(bl_moves, 0x04663315);
}
