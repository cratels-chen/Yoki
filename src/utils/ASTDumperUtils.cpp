#include "ASTDumperUtils.h"
#include "clang/Basic/SourceManager.h"
#include <fstream>
#include <memory>
#include <sstream>

std::string ASTDumperUtils::dumpASTNodeToString(
    const clang::Decl *decl, std::shared_ptr<clang::ASTContext> context) {
  if (!decl) {
    return "null declaration";
  }

  std::string result;
  llvm::raw_string_ostream stream(result);

  try {
    decl->dump(stream);
    stream.flush();
  } catch (...) {
    return "Error: Failed to dump AST node";
  }

  return result;
}

bool ASTDumperUtils::saveASTDumpToFile(
    const clang::Decl *decl, std::shared_ptr<clang::ASTContext> context,
    const std::string &filename) {
  if (!decl) {
    return false;
  }

  try {
    std::ofstream file(filename);
    if (!file.is_open()) {
      return false;
    }

    std::string astDump = dumpASTNodeToString(decl, context);
    file << astDump;
    file.close();
    return true;
  } catch (...) {
    return false;
  }
}

std::string
ASTDumperUtils::getFunctionDetails(const clang::FunctionDecl *func,
                                   std::shared_ptr<clang::ASTContext> context) {
  if (!func) {
    return "null function declaration";
  }

  std::stringstream ss;
  ss << "Function: " << func->getNameAsString() << "\n";
  ss << "Return Type: " << func->getReturnType().getAsString() << "\n";
  ss << "Parameters: " << func->getNumParams() << "\n";

  // 添加位置信息
  if (func->getLocation().isValid()) {
    clang::SourceLocation loc = func->getLocation();
    clang::SourceManager &sm = context->getSourceManager();
    ss << "Location: " << sm.getFilename(loc).str() << ":"
       << sm.getSpellingLineNumber(loc) << "\n";
  }

  // 添加AST dump
  ss << "AST Dump:\n" << dumpASTNodeToString(func, context) << "\n";

  return ss.str();
}
