#ifndef A38285FE_2155_4153_A6EA_8C83946AD862
#define A38285FE_2155_4153_A6EA_8C83946AD862

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include <string>

class ASTDumperUtils {
public:
  // 将AST节点转换为字符串
  static std::string
  dumpASTNodeToString(const clang::Decl *decl,
                      std::shared_ptr<clang::ASTContext> context);

  // 将AST节点保存到文件
  static bool saveASTDumpToFile(const clang::Decl *decl,
                                std::shared_ptr<clang::ASTContext> context,
                                const std::string &filename);

  // 获取函数的详细信息
  static std::string
  getFunctionDetails(const clang::FunctionDecl *func,
                     std::shared_ptr<clang::ASTContext> context);
};

#endif /* A38285FE_2155_4153_A6EA_8C83946AD862 */
