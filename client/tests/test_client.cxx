#include "client.h"
#include "gtest/gtest.h"

TEST(CalcTests, Multiplies) {
  EXPECT_EQ(calc(3, 3), 9);
  EXPECT_EQ(calc(128, 3), 384);
}
