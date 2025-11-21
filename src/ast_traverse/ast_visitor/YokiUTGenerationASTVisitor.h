#ifndef B9A6BE48_F3D8_4505_810D_31AD3F9EE71B
#define B9A6BE48_F3D8_4505_810D_31AD3F9EE71B

#include <clang/AST/AST.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <spdlog/spdlog.h>

using namespace clang;

class YokiUTGenerationASTVisitor
    : public RecursiveASTVisitor<YokiUTGenerationASTVisitor> {
public:
  explicit YokiUTGenerationASTVisitor(ASTContext *context)
      : Context(context), Policy(Context->getPrintingPolicy()) {
    // 设置打印策略以获得更详细的输出
    Policy.PrintCanonicalTypes = true;
    Policy.FullyQualifiedName = true;
    spdlog::info("YokiUTGenerationASTVisitor initialized.");
  }

  bool VisitFunctionDecl(FunctionDecl *node);

private:
  ASTContext *Context;
  PrintingPolicy Policy;

  // 辅助函数：获取函数所在的完整命名空间路径
  std::string getNamespacePrefix(const FunctionDecl *node);

  // 辅助函数：获取函数的完整签名（包括命名空间）
  std::string getFullFunctionSignature(const FunctionDecl *node);
};

#endif /* B9A6BE48_F3D8_4505_810D_31AD3F9EE71B */
