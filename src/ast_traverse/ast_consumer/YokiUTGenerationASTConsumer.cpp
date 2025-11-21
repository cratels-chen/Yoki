#include "YokiUTGenerationASTConsumer.h"

void YokiUTGenerationASTConsumer::HandleTranslationUnit(
    clang::ASTContext &context) {
  visitor.TraverseDecl(context.getTranslationUnitDecl());
}
