#include "FunctionDeclNode.h"

// 获取函数声明
clang::FunctionDecl *FunctionDeclNode::getFunctionDecl() const {
  return functionDecl;
}
// 获取AST上下文
clang::ASTContext *FunctionDeclNode::getContext() const { return context; }
