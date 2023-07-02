#include <gtest/gtest.h>

#include "pdn.h"
#include "history.h"

TEST(PdnTests, Empty)
{
    PDN p;
    EXPECT_TRUE(p._moves.empty());
}

TEST(PdnTests, OneMove)
{
    PDN p;
    const PDN::Move move{{16,20}, false};
    p._moves.push_back(move);
    EXPECT_EQ(p._moves.size(), 1);
    EXPECT_EQ(p._moves.front(), move);
}

TEST(PdnTests, HistoryOneMove)
{
    BoardState s{0x87ff, 0xfff00000, 0, 0, 0, false, -1};
    BoardState t{0x87ff, 0xff780000, 0, 0, 0, true, -1};
    History h;
    h.push(s);
    h.push(t);

    const PDN p = PDN::from_history(h);
    const PDN::Move move{{23,19}, false};

    EXPECT_EQ(p._moves.size(), 1);
    EXPECT_EQ(p._moves.front(), move);
}

TEST(PdnTests, HistoryMoves)
{
    History h;
    h.push(BoardState{0xfff, 0xfff00000, 0, 0, 0, true, -1});
    h.push(BoardState{0x2bff, 0xfff00000, 0, 0, 0, false, -1});
    h.push(BoardState{0x2bff, 0xffb40000, 0, 0, 0, true, -1});
    h.push(BoardState{0x400bff, 0xffb00000, 0, 0, 0, false, -1});
    h.push(BoardState{0xbff, 0xfbb80000, 0, 0, 0, true, -1});

    const PDN p = PDN::from_history(h);
    const PDN::Move move1{{10,13}, false};
    const PDN::Move move2{{22,18}, false};
    const PDN::Move move3{{13,22}, true};
    const PDN::Move move4{{26,19}, true};

    EXPECT_EQ(p._moves.size(), 4);
    EXPECT_EQ(p._moves[0], move1);
    EXPECT_EQ(p._moves[1], move2);
    EXPECT_EQ(p._moves[2], move3);
    EXPECT_EQ(p._moves[3], move4);
}

TEST(PdnTests, HistoryJumpMoves)
{
    History h;
    h.push(BoardState{0x800610, 0xd5420100, 0, 0x100, 0, false, -1});
    h.push(BoardState{0x800600, 0xd5420002, 0, 0x2, 0, false, 1});
    h.push(BoardState{0x800200, 0xd54a0000, 0, 0x80000, 0, true, -1});

    const PDN p = PDN::from_history(h);
    const PDN::Move move{{8, 1, 19}, true};

    EXPECT_EQ(p._moves.size(), 1);
    EXPECT_EQ(p._moves.front(), move);
}

TEST(PdnTests, HistoryStraightJump)
{
    History h;
    h.push(BoardState{0x08000200, 0x00000001, 0, 0x1, 0, false, -1});
    h.push(BoardState{0x08000000, 0x00040000, 0, 0x40000, 0, false, 18});
    h.push(BoardState{0x00000000, 0x80000000, 0, 0x80000000, 0, true, -1});

    const PDN p = PDN::from_history(h);
    const PDN::Move move{{0, 18, 31}, true};

    EXPECT_EQ(p._moves.size(), 1);
    EXPECT_EQ(p._moves.front(), move);
}
