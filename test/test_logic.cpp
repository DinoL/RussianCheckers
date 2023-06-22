#include <gtest/gtest.h>

#include "constants.h"
#include "checkerslogic.h"

TEST(CheckersLogicTests, WhiteTurnFirst)
{
    CheckersLogic logic;
    EXPECT_TRUE(logic.is_white_turn());
}

TEST(CheckersLogicTests, FirstMove)
{
    CheckersLogic logic;
    EXPECT_EQ(logic.current_moves(), 0xf000);
    EXPECT_EQ(logic.current_eat_moves(), 0);
}

TEST(CheckersLogicTests, FilledOnStart)
{
    CheckersLogic logic;
    EXPECT_EQ(logic.filled(), constants::WHITE_START
                            | constants::BLACK_START);
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

TEST(CheckersLogicTests, KingStepMoves)
{
    state_t s = 0x09020000;
    state_t p = 0x0d820200;

    EXPECT_EQ(CheckersLogic::king_step_moves(s, p), 0xd0743548);
}

TEST(CheckersLogicTests, KingEatMovesUnfiltered)
{
    state_t s = 0x80200050;
    state_t b = 0x04061800;
    state_t p = s | b;

    EXPECT_EQ(CheckersLogic::king_eat_moves_unfiltered(s, b, p), 0x4950e200);
}

TEST(CheckersLogicTests, KingEatMovesInDirection)
{
    state_t s = 0x50000004;
    state_t b = 0x00300240;
    state_t p = s | b;

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
    state_t p = s | b;

    EXPECT_EQ(CheckersLogic::king_eat_moves(s, b, p), 0x4150e200);
}

TEST(CheckersLogicTests, WhiteCurrentMoves)
{
    BoardState state{0x00090240,
                     0x14820000,
                     0x200,
                     0,
                     true, -1};
    EXPECT_TRUE(state.valid());

    CheckersLogic logic(state);
    EXPECT_EQ(logic.current_moves(), 0x88543c35);
    EXPECT_EQ(logic.current_eat_moves(), 0);
}

TEST(CheckersLogicTests, BlackCurrentMoves)
{
    BoardState state{0x00014240,
                     0x14820000,
                     0x200,
                     0x04000000,
                     false, -1};
    EXPECT_TRUE(state.valid());

    CheckersLogic logic(state);
    EXPECT_EQ(logic.current_moves(), 0x6368b000);
    EXPECT_EQ(logic.current_eat_moves(), 0);
}

TEST(CheckersLogicTests, WhiteCurrentEatMoves)
{
    BoardState state{0x00090240,
                     0x14920000,
                     0x200,
                     0,
                     true, -1};
    EXPECT_TRUE(state.valid());

    CheckersLogic logic(state);
    EXPECT_EQ(logic.current_moves(), 0x02000000);
    EXPECT_EQ(logic.current_eat_moves(), 0x02000000);
}

TEST(CheckersLogicTests, BlackCurrentEatMoves)
{
    BoardState state{0x00090240,
                     0x14920000,
                     0x200,
                     0,
                     false, -1};
    EXPECT_TRUE(state.valid());

    CheckersLogic logic(state);
    EXPECT_EQ(logic.current_moves(), 0x4000);
    EXPECT_EQ(logic.current_eat_moves(), 0x4000);
}

TEST(CheckersLogicTests, WhiteCurrentKingEatMoves)
{
    BoardState state{0x00090240,
                     0x14920000,
                     0x10200,
                     0,
                     true, -1};
    EXPECT_TRUE(state.valid());

    CheckersLogic logic(state);
    EXPECT_EQ(logic.current_moves(), 0x20000000);
    EXPECT_EQ(logic.current_eat_moves(), 0x20000000);
}

TEST(CheckersLogicTests, BlackCurrentKingEatMoves)
{
    BoardState state{0x00090240,
                     0x14920000,
                     0x200,
                     0x00800000,
                     false, -1};
    EXPECT_TRUE(state.valid());

    CheckersLogic logic(state);
    EXPECT_EQ(logic.current_moves(), 0x420);
    EXPECT_EQ(logic.current_eat_moves(), 0x420);
}

TEST(CheckersLogicTests, WhiteContinueEating)
{
    BoardState state{0x004010a0,
                     0x1c024000,
                     0x20,
                     0x20000,
                     true, 22};
    EXPECT_TRUE(state.valid());

    CheckersLogic logic(state);
    EXPECT_EQ(logic.current_moves(), 0xa0000000);
    EXPECT_EQ(logic.current_eat_moves(), 0xa0a00000);
}

TEST(CheckersLogicTests, BlackContinueEating)
{
    BoardState state{0x004010a0,
                     0x1c024000,
                     0x20,
                     0x20000,
                     false, 26};
    EXPECT_TRUE(state.valid());

    CheckersLogic logic(state);
    EXPECT_EQ(logic.current_moves(), 0x00080000);
    EXPECT_EQ(logic.current_eat_moves(), 0x000c0100);
}
