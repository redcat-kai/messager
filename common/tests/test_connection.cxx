#include "connection.h"
#include "gtest/gtest.h"

TEST(Connection, CanPrint) {
  auto conn = Connection();

  testing::internal::CaptureStdout();
  conn.print();
  auto output = testing::internal::GetCapturedStdout();

  EXPECT_EQ("printing!\n", output);
}
