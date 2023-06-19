#include <iostream>
#include <gtest/gtest.h>

#include "checkerslogic.h"

TEST(CheckersLogicTests, WhiteTurnFirst) {
    CheckersLogic logic;
    EXPECT_TRUE(logic.is_white_turn());
}
