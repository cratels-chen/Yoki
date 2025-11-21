#include "YokiASTFrontendAction.h"
#include "YokiStaticScanASTConsumer.h"
#include "YokiUTGenerationASTConsumer.h"
#include <YokiConfig.h>
#include <clang/Frontend/CompilerInstance.h>
#include <memory>
#include <spdlog/spdlog.h>

std::unique_ptr<clang::ASTConsumer> YokiASTFrontendAction::CreateASTConsumer(
    clang::CompilerInstance &CI, [[maybe_unused]] llvm::StringRef InFile) {
  auto &config = YokiConfig::getInstance();
  auto &context = CI.getASTContext();
  if (config.isStaticAnalysis()) {
    // 如果是静态分析模式，使用 YokiStaticScanASTConsumer
    spdlog::info("Working in static code analysis mode.");
    return std::make_unique<YokiStaticScanASTConsumer>(&context);
  } else if (config.isTUGeneration()) {
    spdlog::info("Working in unit test generation mode.");
    // 如果是单元生成模式，使用 YokiUTGenerationASTConsumer
    return std::make_unique<YokiUTGenerationASTConsumer>(&context);
  } else {
    // 如果没有匹配到任何模式，抛出异常
    spdlog::error("Unsupported mode: {}", config.getMode());
    std::exit(EXIT_FAILURE);
  }
}
