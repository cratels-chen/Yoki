#ifndef C53D3A2E_3217_4FD5_9577_91ACDF0F00EF
#define C53D3A2E_3217_4FD5_9577_91ACDF0F00EF

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
class FunctionDeclNode {
public:
  FunctionDeclNode(clang::FunctionDecl *functionDecl,
                   clang::ASTContext *context)
      : functionDecl(functionDecl), context(context) {}

  // 获取函数声明
  clang::FunctionDecl *getFunctionDecl() const;
  // 获取AST上下文
  clang::ASTContext *getContext() const;

private:
  clang::FunctionDecl *functionDecl = nullptr;
  clang::ASTContext *context = nullptr;
};
#endif /* C53D3A2E_3217_4FD5_9577_91ACDF0F00EF */
