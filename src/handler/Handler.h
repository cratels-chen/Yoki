#ifndef A926F5EA_8E7E_448C_A128_8F0DC2A99BBE
#define A926F5EA_8E7E_448C_A128_8F0DC2A99BBE

#include <clang/Tooling/CompilationDatabase.h>

class Handler {
public:
  static void handle(); // 从YokiConfig获取参数的无参数版本

private:
  static int getThreadSize();

  static void doHandle(std::atomic<int> &proceedFileCount);
};

#endif /* A926F5EA_8E7E_448C_A128_8F0DC2A99BBE */
