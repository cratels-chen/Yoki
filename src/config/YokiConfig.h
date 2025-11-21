#ifndef E32C7F65_C606_4DEE_9B24_B11CA35DAD40
#define E32C7F65_C606_4DEE_9B24_B11CA35DAD40

#include <FunctionDeclNode.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <memory>
#include <string>
#include <vector>

class YokiConfig {
public:
  // 获取单例实例
  static YokiConfig &getInstance();

  // 删除拷贝构造函数和赋值操作符
  YokiConfig(const YokiConfig &) = delete;
  YokiConfig &operator=(const YokiConfig &) = delete;

  std::string getProgramName();
  std::string getProgramPath();
  std::vector<std::string> &getRulesVec();
  std::vector<std::string> &getExcludePaths();
  std::string getMode();

  // CompilationDatabase相关方法
  std::shared_ptr<clang::tooling::CompilationDatabase> getCompilationDB();
  void
  setCompilationDB(std::shared_ptr<clang::tooling::CompilationDatabase> db);

  // FileVec相关方法
  std::vector<std::string> &getFileVec();
  void initializeFileVec(); // 初始化文件列表

  // Mode检查方法
  bool isStaticAnalysis();
  bool isTUGeneration();

  // functionDeclNodes相关方法
  void addFunctionDeclNode(std::shared_ptr<FunctionDeclNode> node) {
    functionDeclNodes.push_back(node);
  }

  void clearFunctionDeclNodes() { functionDeclNodes.clear(); }

  const std::vector<std::shared_ptr<FunctionDeclNode>> &
  getFunctionDeclNodes() const {
    return functionDeclNodes;
  }

  // 加载配置文件
  bool loadConfigFromFile(const std::string &filePath);

private:
  // 私有构造函数
  YokiConfig() = default;

  std::string programName;
  std::string programPath;
  std::vector<std::string> rulesVec;
  std::vector<std::string> excludePaths;
  std::string mode;
  std::shared_ptr<clang::tooling::CompilationDatabase> compilationDBPtr;
  std::vector<std::string> fileVec;
  std::vector<std::shared_ptr<FunctionDeclNode>> functionDeclNodes;
};

#endif /* E32C7F65_C606_4DEE_9B24_B11CA35DAD40 */
