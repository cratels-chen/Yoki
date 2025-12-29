#ifndef C2BDBA7D_F7A1_40EE_8204_DF629E3FB04E
#define C2BDBA7D_F7A1_40EE_8204_DF629E3FB04E
#include "Defect.h"
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class DefectManager {
public:
  static DefectManager &getInstance() {
    static DefectManager instance;
    return instance;
  }

  void addDefect(const Defect &defect);

  // For results that don't originate from a Yoki CheckerBase subclass instance
  // (e.g. Clang Static Analyzer diagnostics). The returned reference stays
  // valid for the lifetime of the process.
  CheckerBase &getOrCreateExternalChecker(const std::string &checkerName,
                                          const std::string &description,
                                          CheckerSeverity severity);

  void dumpAsJson();

  void dumpAsHtml();

  int size() const { return defects.size(); }

private:
  // 禁用拷贝构造函数和赋值运算符
  DefectManager(const DefectManager &) = delete;
  DefectManager &operator=(const DefectManager &) = delete;
  DefectManager() { defects.reserve(1000); }
  ~DefectManager() = default;

  // 辅助方法
  std::string escapeHtml(const std::string &text) const;
  std::string getUniqueFileCount() const;
  std::string getCurrentWorkingDirectory() const;

  std::vector<Defect> defects;

  std::unordered_map<std::string, std::shared_ptr<CheckerBase>>
      externalCheckers;

  std::mutex defectMutex;
};

#endif /* C2BDBA7D_F7A1_40EE_8204_DF629E3FB04E */
