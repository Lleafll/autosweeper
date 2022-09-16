#include "pch.h"
import asw;

TEST(MineFieldTest, RowsAndColumns) {
  asw::MineField mine_field{12, 34};
  EXPECT_EQ(mine_field.rows(), 12);
  EXPECT_EQ(mine_field.columns(), 34);
}

TEST(MineFieldTest, InitiallySetToCleared) {
  asw::MineField const mine_field{1, 2};
  EXPECT_EQ((mine_field(0, 0)), asw::Cell::Empty);
  EXPECT_EQ((mine_field(0, 1)), asw::Cell::Empty);
}

TEST(MineFieldTest, CanSetCells) {
  asw::MineField mine_field{1, 2};
  mine_field(0, 0) = asw::Cell::Mine;
  EXPECT_EQ((mine_field(0, 0)), asw::Cell::Mine);
  EXPECT_EQ((mine_field(0, 1)), asw::Cell::Empty);
}