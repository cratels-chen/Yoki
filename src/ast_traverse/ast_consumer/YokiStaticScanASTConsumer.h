#ifndef FC5C20E8_3E50_433A_A8A4_209A596BBFEA
#define FC5C20E8_3E50_433A_A8A4_209A596BBFEA

#include "YokiStaticScanASTVisitor.h"
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

class YokiStaticScanASTConsumer : public clang::ASTConsumer {
public:
  explicit YokiStaticScanASTConsumer(ASTContext *context) : visitor(context) {}
  virtual void HandleTranslationUnit(clang::ASTContext &context);

private:
  YokiStaticScanASTVisitor visitor;
};

#endif /* FC5C20E8_3E50_433A_A8A4_209A596BBFEA */
