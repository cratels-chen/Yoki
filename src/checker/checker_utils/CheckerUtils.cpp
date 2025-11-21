#include "CheckerUtils.h"

// 将一个整数转为十六进制字符串
std::string intToHex(int value) {
  std::stringstream stream;
  stream << std::hex << std::setw(2) << std::setfill('0') << value;
  return stream.str();
}

// get 16bit hash randomly using time seed
// 生成 16 字符长的哈希值
std::string generateHashID(const std::string &input) {
  // 使用当前时间和输入字符串组合来创建种子
  unsigned long seed = static_cast<unsigned long>(std::time(nullptr));
  for (char c : input) {
    seed ^= c + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }

  // 初始化随机数生成器
  std::mt19937 generator(seed);
  std::uniform_int_distribution<int> distribution(0, 255);

  // 生成 16 字符长的哈希值
  std::string hash;
  for (int i = 0; i < 8; ++i) { // 每个字符占用两个十六进制位，因此生成 8 个整数
    int randomValue = distribution(generator);
    hash += intToHex(randomValue);
  }
  return hash;
}

std::string highlight(const std::string &text) { return "' " + text + " '"; }
