#include <iostream>
#include <gtest/gtest.h>

#include "direction.h"

TEST(DirectionTests, Moves)
{
    state_t s = 0x4c000340;

    EXPECT_EQ(Direction::top_right().moves(s), 0xcc66b800);
    EXPECT_EQ(Direction::top_left().moves(s), 0x61133400);
    EXPECT_EQ(Direction::bottom_right().moves(s), 0x08c8803e);
    EXPECT_EQ(Direction::bottom_left().moves(s), 0x04663315);
}
