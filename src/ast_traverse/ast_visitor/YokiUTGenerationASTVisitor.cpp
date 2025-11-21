#include "YokiUTGenerationASTVisitor.h"
#include <algorithm>
#include <memory>
#include <spdlog/spdlog.h>
#include <vector>

#include "clang/Basic/SourceManager.h"

#include "llvm/Support/raw_ostream.h"
#include <string>

// 辅助函数：获取函数所在的完整命名空间路径
std::string
YokiUTGenerationASTVisitor::getNamespacePrefix(const FunctionDecl *node) {
  std::vector<std::string> namespaces;

  // 遍历从函数到全局作用域的所有上下文
  const DeclContext *DC = node->getDeclContext();
  while (DC && !DC->isTranslationUnit()) {
    if (const auto *ND = dyn_cast<NamespaceDecl>(DC)) {
      // 只添加命名的命名空间（排除匿名命名空间）
      if (!ND->isAnonymousNamespace()) {
        namespaces.push_back(ND->getNameAsString());
      }
    } else if (const auto *RD = dyn_cast<RecordDecl>(DC)) {
      // 处理类/结构体嵌套
      namespaces.push_back(RD->getNameAsString());
    }
    DC = DC->getParent();
  }

  // 反转vector，因为我们是从内到外收集的
  std::reverse(namespaces.begin(), namespaces.end());

  // 构建完整的命名空间前缀
  std::string prefix;
  for (const auto &ns : namespaces) {
    if (!prefix.empty()) {
      prefix += "::";
    }
    prefix += ns;
  }

  return prefix;
}

// 辅助函数：获取函数的完整签名（包括命名空间）
std::string
YokiUTGenerationASTVisitor::getFullFunctionSignature(const FunctionDecl *node) {
  std::string namespacePrefix = getNamespacePrefix(node);
  std::string signature;

  // 添加返回类型
  signature += node->getReturnType().getAsString(Policy) + " ";

  // 添加命名空间前缀（如果有）
  if (!namespacePrefix.empty()) {
    signature += namespacePrefix + "::";
  }

  // 添加函数名
  signature += node->getNameAsString();

  // 添加参数列表
  signature += "(";
  for (unsigned i = 0, e = node->getNumParams(); i != e; ++i) {
    if (i > 0) {
      signature += ", ";
    }
    signature += node->getParamDecl(i)->getType().getAsString(Policy);
    if (!node->getParamDecl(i)->getName().empty()) {
      signature += " " + node->getParamDecl(i)->getName().str();
    }
  }
  signature += ")";

  // 添加const限定符（如果是成员函数）
  if (const auto *MD = dyn_cast<CXXMethodDecl>(node)) {
    if (MD->isConst()) {
      signature += " const";
    }
  }

  return signature;
}

bool YokiUTGenerationASTVisitor::VisitFunctionDecl(FunctionDecl *node) {
  spdlog::info("Visiting function declaration: {}", node->getNameAsString());

  auto funcName = node->getNameAsString();

  // 获取函数所在的命名空间信息
  std::string namespacePrefix = getNamespacePrefix(node);
  std::string fullyQualifiedName = funcName;
  if (!namespacePrefix.empty()) {
    fullyQualifiedName = namespacePrefix + "::" + funcName;
  }

  // 获取完整的函数签名
  std::string fullSignature = getFullFunctionSignature(node);

  spdlog::info("Function namespace: {}",
               namespacePrefix.empty() ? "global" : namespacePrefix);
  spdlog::info("Fully qualified name: {}", fullyQualifiedName);
  spdlog::info("Full signature: {}", fullSignature);
  // // 只收集定义了的函数（有函数体的函数）
  // if (!node->hasBody()) {
  //   spdlog::warn("Function {} does not have a body, skipping.",
  //                node->getNameAsString());
  //   return true; // 返回true表示继续遍历AST
  // }

  // auto functionDeclNode = std::make_shared<FunctionDeclNode>(
  //     node, Context); // 创建FunctionDeclNode实例
  // YokiConfig::getInstance().addFunctionDeclNode(functionDeclNode);
  // spdlog::info("Collected function declaration: {}",
  // node->getNameAsString());
  //     OS << "===== Function Declaration at ";

  std::error_code EC;
  std::unique_ptr<llvm::raw_fd_ostream> OSP =
      std::make_unique<llvm::raw_fd_ostream>(
          fullyQualifiedName + "_ast_dump.yaml", EC);
  // 打印位置信息
  auto &OS = *OSP;
  OS.enable_colors(false); // 禁用颜色输出

  // 添加命名空间信息到输出
  OS << "===== Function Declaration Information =====\n";
  OS << "Function Name: " << funcName << "\n";
  OS << "Namespace: " << (namespacePrefix.empty() ? "global" : namespacePrefix)
     << "\n";
  OS << "Fully Qualified Name: " << fullyQualifiedName << "\n";
  OS << "Full Signature: " << fullSignature << "\n";
  OS << "Location: ";
  FullSourceLoc FullLocation = Context->getFullLoc(node->getBeginLoc());
  spdlog::info("FullLocation: {}", FullLocation.getSpellingLineNumber());
  if (FullLocation.isValid()) {
    OS << FullLocation.getSpellingLineNumber() << ":"
       << FullLocation.getSpellingColumnNumber() << " in "
       << FullLocation.getManager().getFilename(FullLocation).str();
  }
  OS << " =====\n\n";

  spdlog::info("Run here: {}", node->getNameAsString());
  // if (UsePrettyPrint) {
  //   // 方式1: 使用自定义格式化输出
  //   OS << "// Signature:\n";
  //   std::string TypeStr = node->getReturnType().getAsString(Policy) + " ";
  //   TypeStr += node->getNameAsString();
  //   TypeStr += "(";

  //   for (unsigned i = 0, e = node->getNumParams(); i != e; ++i) {
  //     if (i)
  //       TypeStr += ", ";
  //     TypeStr += node->getParamDecl(i)->getType().getAsString(Policy);
  //     if (node->getParamDecl(i)->getName().size())
  //       TypeStr += " " + node->getParamDecl(i)->getName().str();
  //   }

  //   TypeStr += ")";
  //   OS << TypeStr << "\n\n";

  //   // 打印限定符和存储类
  //   if (node->isInlined())
  //     OS << "inline ";
  //   if (node->isVirtualAsWritten())
  //     OS << "virtual ";
  //   if (node->isStatic())
  //     OS << "static ";
  //   if (node->isExternC())
  //     OS << "extern \"C\" ";

  //   // 打印函数体（如果有）
  //   if (node->hasBody()) {
  //     OS << "// Has body: Yes\n";
  //     // 获取函数体的源代码
  //     SourceRange Range = node->getBody()->getSourceRange();
  //     if (Range.isValid()) {
  //       SourceManager &SM = Context->getSourceManager();
  //       OS << "// Body:\n";
  //       OS << Lexer::getSourceText(CharSourceRange::getTokenRange(Range), SM,
  //                                  Context->getLangOpts())
  //                 .str()
  //          << "\n";
  //     }
  //   } else {
  //     OS << "// Has body: No\n";
  //   }

  // } else {
  // 方式2: 使用内置的dump方法
  OS << "// Raw AST Dump:\n";
  node->dump(OS);
  OS << "\n";
  // }

  OS << "\n==========================================================\n\n";
  OS.close();  // 关闭输出流
  return true; // 返回true表示继续遍历AST
}
