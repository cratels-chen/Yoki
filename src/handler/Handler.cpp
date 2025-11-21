#include "Handler.h"
#include "YokiASTFrontendAction.h"
#include "YokiConfig.h"
#include <atomic>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <mutex>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include <vector>

using namespace clang::tooling;
using namespace clang;

int Handler::getThreadSize() {
  // 获取系统的逻辑处理器数量，然后计算线程池大小
  unsigned int num_threads = std::thread::hardware_concurrency() / 2;
  // 防止除以零或零个线程
  num_threads = num_threads > 0 ? num_threads : 1;
  return num_threads;
}

void Handler::handle() {
  // 从YokiConfig单例获取参数
  auto &config = YokiConfig::getInstance();

  std::vector<std::thread> workerPool;

  auto threadSize = Handler::getThreadSize();

  spdlog::info("Yoki using {} thread to complete its work", threadSize);

  std::mutex mtx;
  std::atomic<int> proceedFileCount(0);

  for (int i = 0; i < threadSize; ++i) {
    workerPool.emplace_back(
        std::thread([&] { Handler::doHandle(proceedFileCount); }));
  }

  for (std::thread &worker : workerPool) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void Handler::doHandle(std::atomic<int> &proceedFileCount) {
  auto &config = YokiConfig::getInstance();
  auto compilationDB = config.getCompilationDB();
  auto fileVec = config.getFileVec();

  auto allFileSize = (int)fileVec.size();

  while (true) {
    int currentIndex = proceedFileCount.fetch_add(1);
    if (currentIndex >= allFileSize) {
      break;
    }

    std::string file = fileVec[currentIndex];

    spdlog::info("Thread {} is processing file: {}",
                 std::to_string(
                     std::hash<std::thread::id>{}(std::this_thread::get_id())),
                 file);
    std::vector<std::string> currentFileVec = {file};
    ClangTool Tool(*compilationDB, currentFileVec);
    Tool.run(newFrontendActionFactory<YokiASTFrontendAction>().get());
  }
}
