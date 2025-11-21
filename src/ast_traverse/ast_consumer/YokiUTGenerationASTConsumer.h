#ifndef B1881690_E66F_4CC2_A176_D1C0D7F95697
#define B1881690_E66F_4CC2_A176_D1C0D7F95697

#include "YokiUTGenerationASTVisitor.h"
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <spdlog/spdlog.h>

class YokiUTGenerationASTConsumer : public clang::ASTConsumer {
public:
  explicit YokiUTGenerationASTConsumer(ASTContext *context)
      : visitor(context) {}
  virtual void HandleTranslationUnit(clang::ASTContext &context);

private:
  YokiUTGenerationASTVisitor visitor;
};

#endif /* B1881690_E66F_4CC2_A176_D1C0D7F95697 */
