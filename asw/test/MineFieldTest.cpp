#include "MineField.h"
#include <gtest/gtest.h>

using namespace asw;

namespace {

TEST(MineFieldTest, RowsAndColumns) {
    MineField mine_field{12, 34};
    EXPECT_EQ(mine_field.rows(), 12);
    EXPECT_EQ(mine_field.columns(), 34);
}

TEST(MineFieldTest, InitiallySetToCleared) {
    MineField const mine_field{1, 2};
    EXPECT_EQ((mine_field(0, 0)), MineField::Cell::Empty);
    EXPECT_EQ((mine_field(0, 1)), MineField::Cell::Empty);
}

TEST(MineFieldTest, CanSetCells) {
    MineField mine_field{1, 2};
    mine_field(0, 0) = MineField::Cell::Mine;
    EXPECT_EQ((mine_field(0, 0)), MineField::Cell::Mine);
    EXPECT_EQ((mine_field(0, 1)), MineField::Cell::Empty);
}

}  // namespace