#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTDumper.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <cstdlib>
#include <memory>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <system_error>

#include "CheckerManager.h"
#include "DefectManager.h"
#include "Handler.h"
#include "YokiConfig.h"
#include "compliance_public_header.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

using namespace clang;
using namespace clang::tooling;

// Tip: 不定义该变量会导致链接错误
int llvm::DisableABIBreakingChecks = 1;

// 自定义命令行集合，用于将命令行选项分组
static llvm::cl::OptionCategory MyToolCategory("Yoki options");

int main(int argc, const char **argv) {
  // 设置日志级别为 info
  spdlog::set_level(spdlog::level::info);

  spdlog::info("Yoki static analysis tool started.");

  // 清除默认options
  llvm::cl::HideUnrelatedOptions(MyToolCategory);

  // 如果没有提供任何参数，添加--help到参数列表
  std::vector<const char *> modified_argv;
  int modified_argc = argc;
  const char **final_argv = argv;

  if (argc == 1) {
    modified_argv.push_back(argv[0]);  // 程序名
    modified_argv.push_back("--help"); // 添加--help参数
    modified_argc = 2;
    final_argv = modified_argv.data();
  }

  // 定义一个命令行选项，并将其归类到自定义类别中
  static llvm::cl::opt<std::string> configFilePath(
      llvm::cl::Positional, llvm::cl::desc("Path to configuration file"),
      llvm::cl::Required, llvm::cl::cat(MyToolCategory));

  // 处理命令行参数
  llvm::cl::ParseCommandLineOptions(modified_argc, final_argv, "Yoki\n");

  spdlog::info("Path to configuration file: " + configFilePath);

  auto &config = YokiConfig::getInstance();

  // 读取配置文件
  if (!config.loadConfigFromFile(configFilePath)) {
    spdlog::error("Failed to load configuration file: {}", configFilePath);
    return 1;
  }

  // 根据配置文件中的项目路径找到compile_command.json文件
  std::string compileCommandDir = config.getProgramPath() + "/build";
  spdlog::info("Compile command directory: " + compileCommandDir);

  // 加载compile_commands.json文件生成CompilationDatabase
  std::string errorMsg = "";
  auto compilationDB = clang::tooling::CompilationDatabase::loadFromDirectory(
      compileCommandDir, errorMsg);

  if (!compilationDB) {
    spdlog::error("Failed to load compile_commands.json: {}", errorMsg);
    std::exit(-1);
  }
  spdlog::info("Loaded compile_commands.json successfully.");

  // complilationDB是一个unique_ptr类型智能指针，使用std::move将其转移到shared_ptr中
  // 并存储到YokiConfig中
  std::shared_ptr compilationDBPtr = std::move(compilationDB);
  config.setCompilationDB(compilationDBPtr);

  // 初始化文件列表
  config.initializeFileVec();

  if (config.getFileVec().empty()) {
    spdlog::error("No files to be checked.");
    std::exit(EXIT_FAILURE);
  }

  if (config.isStaticAnalysis()) {
    auto &checkerManager = CheckerManager::getInstance();

    // 初始化检查器，将所有系统支持的检查器全部注册到checkerManager中
    checkerManager.initializeCheckers();

    // 根据用户的配置文件重置checkerManager中的检查器
    // 若用户未显式配置，则默认启用所有检查器
    checkerManager.setUpEnabledCheckers();
    spdlog::info("Enabled checkers: " +
                 std::to_string(checkerManager.getEnabledCheckers().size()));
    // 输出启用的检查器列表
    for (auto &checker : checkerManager.getEnabledCheckers()) {
      spdlog::info("---- {}", checker->getName());
    }

    Handler::handle();
    spdlog::info("Static code analysis finished.");

    // 输出分析结果
    spdlog::info("Size of defects: {}", DefectManager::getInstance().size());
    auto &defectManager = DefectManager::getInstance();

    // 只有在有缺陷时才生成报告
    if (defectManager.size() > 0) {
      spdlog::info("Generating HTML report...");
      try {
        // defectManager.dumpAsJson();
        defectManager.dumpAsHtml();
        spdlog::info("HTML report generated successfully.");
      } catch (const std::exception &e) {
        spdlog::error("Failed to generate HTML report: {}", e.what());
      }
    } else {
      spdlog::info("No defects found, skipping report generation.");
    }
  }

  if (config.isTUGeneration()) {
    Handler::handle();
  }

  spdlog::info("Yoki finished running.");
  return 0;
}
