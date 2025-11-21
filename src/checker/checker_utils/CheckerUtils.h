#ifndef E0475687_9D14_462E_9FFD_DE95C636A8AB
#define E0475687_9D14_462E_9FFD_DE95C636A8AB

#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#define __CHECKER_CTOR__(checker_name)                                         \
  checker_name(std::string name, std::string description,                      \
               CheckerSeverity category)                                       \
      : CheckerBase(name, description, category) {}

enum class CheckerSeverity { ADVISORY, REQUIRED, MANDATORY };

// 将一个整数转为十六进制字符串
std::string intToHex(int value);

// get 16bit hash randomly using time seed
// 生成 16 字符长的哈希值
std::string generateHashID(const std::string &input);

std::string highlight(const std::string &text);

#endif /* E0475687_9D14_462E_9FFD_DE95C636A8AB */
