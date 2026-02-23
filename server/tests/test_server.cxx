#include "server.h"
#include "gtest/gtest.h"

TEST(IsEvenTests, HandlesEven) {
  EXPECT_TRUE(is_even(2));
}

TEST(IsEvenTests, HandlesOdd) {
  EXPECT_FALSE(is_even(3));
}
