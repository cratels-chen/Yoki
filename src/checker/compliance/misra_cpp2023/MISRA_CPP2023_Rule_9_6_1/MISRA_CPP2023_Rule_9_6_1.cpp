#include "MISRA_CPP2023_Rule_9_6_1.h"
#include "CheckerRegistry.h"
#include "Defect.h"
#include "DefectManager.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include <string>

YOKI_REGISTER_CHECKER(misra_cpp2023, MISRA_CPP2023_Rule_9_6_1,
                      "MISRA_CPP2023:Rule 9.6.1",
                      "A goto statement shall not be used.", ADVISORY);

bool MISRA_CPP2023_Rule_9_6_1::VisitGotoStmt(GotoStmt *node,
                                             ASTContext *context) {
  auto defectMessage = highlight("goto") + "statement should not be used.";
  auto id = generateHashID(defectMessage);

  auto &sourceManager = context->getSourceManager();
  auto beginLoc = node->getBeginLoc();

  const auto &filePath = sourceManager.getFilename(beginLoc).str();
  auto lineNumber = sourceManager.getSpellingLineNumber(beginLoc);

  Defect defect(id, *this, defectMessage, filePath, lineNumber);

  DefectManager::getInstance().addDefect(defect);

  return true;
}
