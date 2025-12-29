#include "CheckerManager.h"
#include "../config/YokiConfig.h"
#include "CheckerRegistry.h"
#include "CheckerUtils.h"
#include <memory>
#include <spdlog/spdlog.h>

void CheckerManager::initializeCheckers() {
  for (const auto &entry : CheckerRegistry::getInstance().getAll()) {
    supportCheckerVec.emplace_back(entry.factory());
  }
}

#define __YOKI_VISIT_NODE__(NODE)                                              \
  bool CheckerManager::Visit##NODE(NODE *node, ASTContext *context) {          \
    for (const auto &checker : enabledCheckerVec) {                            \
      if (checker->Visit##NODE(node, context)) {                               \
        return true;                                                           \
      }                                                                        \
    }                                                                          \
    return false;                                                              \
  }
#include "visit_node.inc"
#undef __YOKI_VISIT_NODE__

void CheckerManager::setUpEnabledCheckers(
    const std::vector<std::string> &rulesVec) {
  if (!rulesVec.empty()) {

    for (const auto &name : rulesVec) {
      bool found = false;
      for (auto &checker : supportCheckerVec) {
        if (checker->getName() == name) {
          enabledCheckerVec.push_back(checker);
          found = true;
          break;
        }
      }
      if (!found) {
        spdlog::warn("Checker {} not found in support checkers", name);
      }
    }
  } else {
    // 如果用户没有显式配置，则默认启用所有检查器
    enabledCheckerVec = supportCheckerVec;
    spdlog::info("No rules specified, enabling all checkers.");
  }
}

void CheckerManager::setUpEnabledCheckers() {
  // 从YokiConfig单例获取规则
  auto &config = YokiConfig::getInstance();
  setUpEnabledCheckers(config.getRulesVec());
}
