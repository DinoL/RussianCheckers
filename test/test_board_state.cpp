#include <iostream>
#include <gtest/gtest.h>

#include "constants.h"
#include "boardstate.h"

TEST(BoardStateTests, WhiteTurnFirst)
{
    BoardState state;
    EXPECT_TRUE(state._white_turn);
}

TEST(BoardStateTests, FilledOnStart)
{
    BoardState state;
    EXPECT_EQ(state.filled(), constants::WHITE_START
                            | constants::BLACK_START);
}

TEST(BoardStateTests, ClearCells)
{
    BoardState state1;
    state1.clear_cells(0xffffffff);
    EXPECT_EQ(state1.filled(), 0);

    BoardState state2{0x5913a01a,
                      0xa2481b24,
                      0x00108002,
                      0x02001100,
                      true, -1};
    state2.clear_cells(0x366cc993);
    EXPECT_EQ(state2.filled(), 0xc913322c);
}
