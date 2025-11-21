#include "f3_function.h"
#include <chrono>
#include <gtest/gtest.h>
#include <tuple>

// 测试类，用于组织 f3 函数的测试用例
class F3FunctionTest : public ::testing::Test {
protected:
  void SetUp() override {
    // 在每个测试用例执行前的设置
  }

  void TearDown() override {
    // 在每个测试用例执行后的清理
  }
};

// 测试偶数输入的情况
TEST_F(F3FunctionTest, EvenNumberInput) {
  // 测试 a 为偶数时，结果应该是 a + b
  EXPECT_EQ(A::B::f3(2, 3), 5);  // 2 + 3 = 5
  EXPECT_EQ(A::B::f3(4, 6), 10); // 4 + 6 = 10
  EXPECT_EQ(A::B::f3(0, 5), 5);  // 0 + 5 = 5
  EXPECT_EQ(A::B::f3(-2, 7), 5); // -2 + 7 = 5
  EXPECT_EQ(A::B::f3(8, -3), 5); // 8 + (-3) = 5
}

// 测试奇数输入的情况
TEST_F(F3FunctionTest, OddNumberInput) {
  // 测试 a 为奇数时，结果应该是 a + b + 1
  EXPECT_EQ(A::B::f3(1, 3), 5);  // 1 + 3 + 1 = 5
  EXPECT_EQ(A::B::f3(3, 4), 8);  // 3 + 4 + 1 = 8
  EXPECT_EQ(A::B::f3(5, 0), 6);  // 5 + 0 + 1 = 6
  EXPECT_EQ(A::B::f3(-1, 2), 2); // -1 + 2 + 1 = 2
  EXPECT_EQ(A::B::f3(7, -5), 3); // 7 + (-5) + 1 = 3
}

// 边界值测试
TEST_F(F3FunctionTest, BoundaryValues) {
  // 测试边界值和特殊情况
  EXPECT_EQ(A::B::f3(0, 0), 0); // 0 是偶数，0 + 0 = 0
  EXPECT_EQ(A::B::f3(1, 0), 2); // 1 是奇数，1 + 0 + 1 = 2

  // 测试负数边界
  EXPECT_EQ(A::B::f3(-2, 0), -2); // -2 是偶数，-2 + 0 = -2
  EXPECT_EQ(A::B::f3(-1, 0), 0);  // -1 是奇数，-1 + 0 + 1 = 0
}

// 参数化测试：使用多组数据进行测试
class F3ParameterizedTest
    : public ::testing::TestWithParam<std::tuple<int, int, int>> {};

TEST_P(F3ParameterizedTest, ParameterizedTestCases) {
  auto [a, b, expected] = GetParam();
  EXPECT_EQ(A::B::f3(a, b), expected);
}

// 参数化测试数据
INSTANTIATE_TEST_SUITE_P(F3TestData, F3ParameterizedTest,
                         ::testing::Values(
                             // 偶数测试用例 (a, b, expected)
                             std::make_tuple(2, 3, 5),   // 2 + 3 = 5
                             std::make_tuple(4, 6, 10),  // 4 + 6 = 10
                             std::make_tuple(0, 5, 5),   // 0 + 5 = 5
                             std::make_tuple(-4, 2, -2), // -4 + 2 = -2

                             // 奇数测试用例 (a, b, expected)
                             std::make_tuple(1, 3, 5),  // 1 + 3 + 1 = 5
                             std::make_tuple(3, 4, 8),  // 3 + 4 + 1 = 8
                             std::make_tuple(5, 0, 6),  // 5 + 0 + 1 = 6
                             std::make_tuple(-3, 1, -1) // -3 + 1 + 1 = -1
                             ));

// 性能测试（如果需要）
TEST_F(F3FunctionTest, PerformanceTest) {
  // 测试函数执行时间（虽然这个函数很简单，但可以作为示例）
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 1000000; ++i) {
    A::B::f3(i, i + 1);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  // 断言执行时间应该在合理范围内（比如小于100毫秒）
  EXPECT_LT(duration.count(), 100) << "Function f3 performance test failed";
}

// 主函数
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
