#include <gtest/gtest.h>

#include "history.h"

TEST(HistoryTests, Empty)
{
    History h;
    EXPECT_TRUE(h.empty());
    EXPECT_EQ(h.size(), 0);
}

TEST(HistoryTests, Size)
{
    BoardState s;
    History h;
    for (int i = 0; i < 10; ++i)
    {
        h.push(s);
    }
    EXPECT_FALSE(h.empty());
    EXPECT_EQ(h.size(), 10);
}

TEST(HistoryTests, Top)
{
    BoardState s;
    BoardState t{0x87ff, 0xfff00000, 0, 0, 0, false, -1};
    History h;
    h.push(s);
    h.push(t);
    EXPECT_FALSE(h.empty());
    EXPECT_EQ(h.size(), 2);
    EXPECT_EQ(h.top(), t);
}

TEST(HistoryTests, Pop)
{
    BoardState s{0x87ff, 0xfff00000, 0, 0, 0, false, -1};
    BoardState t{0x87ff, 0xff780000, 0, 0, 0, true, -1};
    History h;
    h.push(s);
    h.push(t);
    h.pop();
    EXPECT_FALSE(h.empty());
    EXPECT_EQ(h.size(), 1);
    EXPECT_EQ(h.top(), s);
}

TEST(HistoryTests, PopLast)
{
    BoardState s{0x87ff, 0xfff00000, 0, 0, 0, false, -1};
    History h;
    h.push(s);
    h.pop();
    EXPECT_TRUE(h.empty());
}

TEST(HistoryTests, Clear)
{
    BoardState s{0x87ff, 0xfff00000, 0, 0, 0, false, -1};
    BoardState t{0x87ff, 0xff780000, 0, 0, 0, true, -1};
    BoardState u{0x96ff, 0xff780000, 0, 0, 0, false, -1};
    History h;
    h.push(s);
    h.push(t);
    h.push(u);
    h.clear();
    EXPECT_TRUE(h.empty());
}

TEST(HistoryTests, GetItem)
{
    BoardState s{0x87ff, 0xfff00000, 0, 0, 0, false, -1};
    BoardState t{0x87ff, 0xff780000, 0, 0, 0, true, -1};
    BoardState u{0x96ff, 0xff780000, 0, 0, 0, false, -1};
    History h;
    h.push(s);
    h.push(t);
    h.push(u);
    EXPECT_FALSE(h.empty());
    EXPECT_EQ(h.size(), 3);
    EXPECT_EQ(h[0], s);
    EXPECT_EQ(h[1], t);
    EXPECT_EQ(h[2], u);
}
