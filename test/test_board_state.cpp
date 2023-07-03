#include <gtest/gtest.h>

#include "constants.h"
#include "boardstate.h"

TEST(BoardStateTests, ValidOnStart)
{
    BoardState state;
    EXPECT_TRUE(state.valid());
}

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

TEST(BoardStateTests, EatFromCells)
{
    BoardState state1;
    state1.eat_from_cells(0xffffffff);
    EXPECT_EQ(state1._white, 0);
    EXPECT_EQ(state1._black, 0);
    EXPECT_EQ(state1._eaten, 0xfff00000);
    EXPECT_TRUE(state1.valid());

    BoardState state2{0x5913a01a,
                      0xa2481b24,
                      0x00108002,
                      0x02001100,
                      0, true, -1};
    state2.eat_from_cells(0x366cc993);
    EXPECT_EQ(state2._white, 0x49132008);
    EXPECT_EQ(state2._black, 0x80001224);
    EXPECT_EQ(state2._eaten, 0x22480900);
    EXPECT_TRUE(state2.valid());
}

TEST(BoardStateTests, MovePiece)
{
    BoardState state{0x5913a01a,
                     0xa2481b24,
                     0x00108002,
                     0x02001100,
                     0, false, -1};
    EXPECT_TRUE(state.valid());
    state.move_piece(12, 21);

    BoardState expected{0x5913a01a,
                        0xa2680b24,
                        0x00108002,
                        0x02200100,
                        0, false, -1};
    EXPECT_EQ(state, expected);
    EXPECT_TRUE(state.valid());
}

TEST(BoardStateTests, BecomeKing)
{
    BoardState state{0x04000000,
                     0x40,
                     0,
                     0,
                     0, true, -1};
    EXPECT_TRUE(state.valid());
    state.move_piece(26, 29);
    state.move_piece(6, 3);

    BoardState expected{0x20000000,
                        0x8,
                        0x20000000,
                        0x8,
                        0, true, -1};
    EXPECT_EQ(state, expected);
    EXPECT_TRUE(state.valid());
}

TEST(BoardStateTests, InvalidWhiteBlackIntersect)
{
    BoardState state{0xabcdef85,
                     0x5432117a,
                     0,
                     0,
                     0, true, -1};
    EXPECT_FALSE(state.valid());
    state._black &= ~0x100;
    EXPECT_TRUE(state.valid());
}

TEST(BoardStateTests, InvalidWhiteKingOutside)
{
    BoardState state{0xabcdef85,
                     0x54321072,
                     0x8,
                     0,
                     0, true, -1};
    EXPECT_FALSE(state.valid());
    state._white |= state._white_kings;
    EXPECT_TRUE(state.valid());
}


TEST(BoardStateTests, InvalidBlackKingOutside)
{
    BoardState state{0xabcdef85,
                     0x54321072,
                     0,
                     0x8,
                     0, true, -1};
    EXPECT_FALSE(state.valid());
    state._black |= state._black_kings;
    EXPECT_TRUE(state.valid());
}

TEST(BoardStateTests, InvalidTooLowEatingPiece)
{
    BoardState state{0xabcdef85,
                     0x5432107a,
                     0x5,
                     0x8,
                     0, false, -2};
    EXPECT_FALSE(state.valid());
    state._eating_piece = -1;
    EXPECT_TRUE(state.valid());
}

TEST(BoardStateTests, InvalidTooHighEatingPiece)
{
    BoardState state{0xabcdef85,
                     0x5432107a,
                     0x5,
                     0x8,
                     0, true, 32};
    EXPECT_FALSE(state.valid());
    state._eating_piece = 31;
    EXPECT_TRUE(state.valid());
}

TEST(BoardStateTests, InvalidEatingPieceEmpty)
{
    BoardState state{0xabcdef85,
                     0x54321072,
                     0x5,
                     0x2,
                     0, false, 3};
    EXPECT_FALSE(state.valid());
    state._black |= alg::to_state(state._eating_piece);
    EXPECT_TRUE(state.valid());
}

TEST(BoardStateTests, InvalidEatingPieceWrongColor)
{
    BoardState state{0xabcdef85,
                     0x5432107a,
                     0x5,
                     0x2,
                     0, true, 3};
    EXPECT_FALSE(state.valid());
    state._eating_piece = 2;
    EXPECT_TRUE(state.valid());
}
