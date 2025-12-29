#ifndef YOKI_CHECKER_REGISTRY_H
#define YOKI_CHECKER_REGISTRY_H

#include "CheckerBase.h"

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

class CheckerRegistry {
public:
  struct Entry {
    std::string standardName;
    std::string checkerName;
    std::string description;
    CheckerSeverity severity;
    std::function<std::shared_ptr<CheckerBase>()> factory;
  };

  static CheckerRegistry &getInstance();

  void registerChecker(Entry entry);

  std::vector<Entry> getAll() const;

private:
  CheckerRegistry() = default;
  CheckerRegistry(const CheckerRegistry &) = delete;
  CheckerRegistry &operator=(const CheckerRegistry &) = delete;

  mutable std::mutex mtx;
  std::vector<Entry> entries;
};

#define YOKI_REGISTER_CHECKER(STANDARD_NAME, CLASS_NAME, CHECKER_NAME, DESC,   \
                              SEVERITY)                                        \
  namespace {                                                                  \
  struct YokiCheckerAutoRegister_##CLASS_NAME {                                \
    YokiCheckerAutoRegister_##CLASS_NAME() {                                   \
      CheckerRegistry::getInstance().registerChecker(CheckerRegistry::Entry{   \
          #STANDARD_NAME, CHECKER_NAME, DESC, CheckerSeverity::SEVERITY,       \
          []() -> std::shared_ptr<CheckerBase> {                               \
            return std::make_shared<CLASS_NAME>(CHECKER_NAME, DESC,            \
                                                CheckerSeverity::SEVERITY);    \
          }});                                                                 \
    }                                                                          \
  };                                                                           \
  static YokiCheckerAutoRegister_##CLASS_NAME                                  \
      g_yokiCheckerAutoRegister_##CLASS_NAME;                                  \
  } // namespace

#endif
