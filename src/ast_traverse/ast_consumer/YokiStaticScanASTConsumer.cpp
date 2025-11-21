#include "YokiStaticScanASTConsumer.h"

void YokiStaticScanASTConsumer::HandleTranslationUnit(
    clang::ASTContext &context) {
  visitor.TraverseDecl(context.getTranslationUnitDecl());
}
