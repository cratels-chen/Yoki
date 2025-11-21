#include "MISRA_CPP2023_Rule_9_6_1.h"
#include "Defect.h"
#include "DefectManager.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
#include <string>

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
