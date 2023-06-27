#include <gtest/gtest.h>

#include "pdnparser.h"
#include "pdn.h"

TEST(CellParserTests, CellParserEncodeNumeric)
{
    const CellParser parser(CellFormat::NUMERIC);
    EXPECT_EQ(parser.encode(0), "1");
    EXPECT_EQ(parser.encode(9), "10");
    EXPECT_EQ(parser.encode(24), "25");
    EXPECT_EQ(parser.encode(31), "32");
}

TEST(CellParserTests, CellParserDecodeNumeric)
{
    const CellParser parser(CellFormat::NUMERIC);
    EXPECT_EQ(parser.decode("1"), 0);
    EXPECT_EQ(parser.decode("10"), 9);
    EXPECT_EQ(parser.decode("25"), 24);
    EXPECT_EQ(parser.decode("32"), 31);
}

TEST(CellParserTests, CellParserEncodeAlphanumeric)
{
    const CellParser parser(CellFormat::ALPHANUMERIC);
    EXPECT_EQ(parser.encode(0), "a1");
    EXPECT_EQ(parser.encode(9), "c3");
    EXPECT_EQ(parser.encode(21), "d6");
    EXPECT_EQ(parser.encode(31), "h8");
}

TEST(CellParserTests, CellParserDecodeAlphanumeric)
{
    const CellParser parser(CellFormat::ALPHANUMERIC);
    EXPECT_EQ(parser.decode("a1"), 0);
    EXPECT_EQ(parser.decode("c3"), 9);
    EXPECT_EQ(parser.decode("d6"), 21);
    EXPECT_EQ(parser.decode("h8"), 31);
}

TEST(MoveParserTests, MoveParserEncodeNumeric)
{
    const MoveParser parser(CellFormat::NUMERIC);
    EXPECT_EQ(parser.encode(PDN::Move{{0, 4}, false}), "1-5");
    EXPECT_EQ(parser.encode(PDN::Move{{17, 20}, false}), "18-21");
    EXPECT_EQ(parser.encode(PDN::Move{{30, 8}, false}), "31-9");
    EXPECT_EQ(parser.encode(PDN::Move{{11, 7}, false}), "12-8");
}

TEST(MoveParserTests, MoveParserEncodeEatNumeric)
{
    const MoveParser parser(CellFormat::NUMERIC);
    EXPECT_EQ(parser.encode(PDN::Move{{0, 9}, true}), "1x10");
    EXPECT_EQ(parser.encode(PDN::Move{{17, 24}, true}), "18x25");
    EXPECT_EQ(parser.encode(PDN::Move{{30, 8}, true}), "31x9");
    EXPECT_EQ(parser.encode(PDN::Move{{14, 7}, true}), "15x8");
    EXPECT_EQ(parser.encode(PDN::Move{{12, 26, 19}, true}), "13x27x20");
    EXPECT_EQ(parser.encode(PDN::Move{{28, 11, 2, 16}, true}), "29x12x3x17");
}

TEST(MoveParserTests, MoveParserDecodeNumeric)
{
    const MoveParser parser(CellFormat::NUMERIC);
    EXPECT_EQ(parser.decode("1-5"), PDN::Move({0, 4}, false));
    EXPECT_EQ(parser.decode("18-21"), PDN::Move({17, 20}, false));
    EXPECT_EQ(parser.decode("31-9"), PDN::Move({30, 8}, false));
    EXPECT_EQ(parser.decode("12-8"), PDN::Move({11, 7}, false));
}

TEST(MoveParserTests, MoveParserDecodeEatNumeric)
{
    const MoveParser parser(CellFormat::NUMERIC);
    EXPECT_EQ(parser.decode("1x10"), PDN::Move({0, 9}, true));
    EXPECT_EQ(parser.decode("18x25"), PDN::Move({17, 24}, true));
    EXPECT_EQ(parser.decode("31x9"), PDN::Move({30, 8}, true));
    EXPECT_EQ(parser.decode("15x8"), PDN::Move({14, 7}, true));
    EXPECT_EQ(parser.decode("13x27x20"), PDN::Move({12, 26, 19}, true));
    EXPECT_EQ(parser.decode("29x12x3x17"), PDN::Move({28, 11, 2, 16}, true));
}

TEST(MoveParserTests, MoveParserDecodeAlphanumeric)
{
    const MoveParser parser(CellFormat::ALPHANUMERIC);
    EXPECT_EQ(parser.decode("a1-b2"), PDN::Move({0, 4}, false));
    EXPECT_EQ(parser.decode("c5-b6"), PDN::Move({17, 20}, false));
    EXPECT_EQ(parser.decode("f8-a3"), PDN::Move({30, 8}, false));
    EXPECT_EQ(parser.decode("g3-h2"), PDN::Move({11, 7}, false));
}

TEST(MoveParserTests, MoveParserDecodeEatAlphanumeric)
{
    const MoveParser parser(CellFormat::ALPHANUMERIC);
    EXPECT_EQ(parser.decode("a1xc3"), PDN::Move({0, 9}, true));
    EXPECT_EQ(parser.decode("c5xa7"), PDN::Move({17, 24}, true));
    EXPECT_EQ(parser.decode("f8xa3"), PDN::Move({30, 8}, true));
    EXPECT_EQ(parser.decode("f4xh2"), PDN::Move({14, 7}, true));
    EXPECT_EQ(parser.decode("b4xe7xg5"), PDN::Move({12, 26, 19}, true));
    EXPECT_EQ(parser.decode("b8xg3xe1xa5"), PDN::Move({28, 11, 2, 16}, true));
}
