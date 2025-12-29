#include "Handler.h"
#include "CheckerUtils.h"
#include "Defect.h"
#include "DefectManager.h"
#include "YokiASTFrontendAction.h"
#include "YokiConfig.h"
#include <atomic>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>
#include <clang/StaticAnalyzer/Frontend/FrontendActions.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/ADT/SmallString.h>
#include <mutex>
#include <regex>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include <vector>

using namespace clang::tooling;
using namespace clang;

namespace {
class ClangAnalyzerDiagnosticConsumer final : public clang::DiagnosticConsumer {
public:
  void HandleDiagnostic(clang::DiagnosticsEngine::Level level,
                        const clang::Diagnostic &info) override {
    if (level != clang::DiagnosticsEngine::Warning &&
        level != clang::DiagnosticsEngine::Error &&
        level != clang::DiagnosticsEngine::Fatal) {
      return;
    }

    llvm::SmallString<256> formatted;
    info.FormatDiagnostic(formatted);
    const std::string message = formatted.str().str();

    // Static analyzer diagnostics usually end with:
    //   "... (clang-analyzer-core.NullDereference)"
    static const std::regex checkerRe(R"(\((clang-analyzer[^\)]*)\)\s*$)");
    std::smatch match;
    if (!std::regex_search(message, match, checkerRe)) {
      return; // Ignore non-static-analyzer diagnostics.
    }
    const std::string checkerName = match[1].str();

    std::string filePath;
    int lineNumber = 0;
    if (info.hasSourceManager()) {
      auto &sm = info.getSourceManager();
      auto loc = sm.getSpellingLoc(info.getLocation());
      if (loc.isValid()) {
        filePath = sm.getFilename(loc).str();
        lineNumber = (int)sm.getSpellingLineNumber(loc);
      }
    }

    // Best-effort: some analyzer diagnostics may not have a file location.
    if (filePath.empty()) {
      return;
    }

    const std::string defectId =
        generateHashID(checkerName + "|" + filePath + "|" +
                       std::to_string(lineNumber) + "|" + message);

    auto &checker = DefectManager::getInstance().getOrCreateExternalChecker(
        checkerName, "Clang Static Analyzer", CheckerSeverity::ADVISORY);
    Defect defect(defectId, checker, message, filePath, lineNumber);
    DefectManager::getInstance().addDefect(defect);
  }
};
} // namespace

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

    // 1) Run Yoki's custom AST-based checks.
    {
      ClangTool tool(*compilationDB, currentFileVec);
      tool.run(newFrontendActionFactory<YokiASTFrontendAction>().get());
    }

    // 2) Run Clang Static Analyzer with all built-in checkers enabled.
    {
      ClangTool analyzerTool(*compilationDB, currentFileVec);

      ClangAnalyzerDiagnosticConsumer diagConsumer;
      analyzerTool.setDiagnosticConsumer(&diagConsumer);

      analyzerTool.appendArgumentsAdjuster(
          [](const CommandLineArguments &args, StringRef) {
            CommandLineArguments adjusted = args;
            adjusted.push_back("-Xclang");
            adjusted.push_back("-analyzer-checker=all");
            // Ensure analyzer diagnostics include checker names.
            adjusted.push_back("-Xclang");
            adjusted.push_back("-analyzer-output=text");
            return adjusted;
          });

      analyzerTool.run(
          newFrontendActionFactory<clang::ento::AnalysisAction>().get());
    }
  }
}
