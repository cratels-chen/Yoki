#ifndef E729D96B_D8AD_416B_9C9B_B5BB890194AF
#define E729D96B_D8AD_416B_9C9B_B5BB890194AF

#include <clang/Frontend/FrontendActions.h>
#include <memory>

class YokiASTFrontendAction : public clang::ASTFrontendAction {
public:
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &CI,
                    llvm::StringRef InFile) override;
};

#endif /* E729D96B_D8AD_416B_9C9B_B5BB890194AF */
