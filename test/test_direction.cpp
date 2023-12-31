#include <gtest/gtest.h>

#include "direction.h"

TEST(DirectionTests, Move)
{
    state_t s = 0x4000;

    EXPECT_EQ(Direction::top_right().move(s), 0x80000);
    EXPECT_EQ(Direction::top_left().move(s), 0x40000);
    EXPECT_EQ(Direction::bottom_right().move(s), 0x800);
    EXPECT_EQ(Direction::bottom_left().move(s), 0x400);
}

TEST(DirectionTests, AllMoves)
{
    state_t s = 0x4c000340;

    EXPECT_EQ(Direction::top_right().all_moves(s), 0xcc66b800);
    EXPECT_EQ(Direction::top_left().all_moves(s), 0x61133400);
    EXPECT_EQ(Direction::bottom_right().all_moves(s), 0x08c8803e);
    EXPECT_EQ(Direction::bottom_left().all_moves(s), 0x04663315);
}

TEST(DirectionTests, FreeMoves)
{
    state_t s = 0x00400400;
    state_t p = 0x00188208;
    EXPECT_EQ(Direction::top_right().free_moves(s, p), 0x88004000);
    EXPECT_EQ(Direction::top_left().free_moves(s, p), 0x24022000);
    EXPECT_EQ(Direction::bottom_right().free_moves(s, p), 0x40);
    EXPECT_EQ(Direction::bottom_left().free_moves(s, p), 0x00042022);
}

TEST(DirectionTests, EatMoves)
{
    state_t s = 0x80200050;
    state_t b = 0x04061800;
    state_t p = 0x84261850;

    EXPECT_EQ(Direction::top_right().eat_moves(s, b, p), 0x48408000);
    EXPECT_EQ(Direction::top_left().eat_moves(s, b, p), 0x01100000);
    EXPECT_EQ(Direction::bottom_right().eat_moves(s, b, p), 0x00004000);
    EXPECT_EQ(Direction::bottom_left().eat_moves(s, b, p), 0x00002200);
}
