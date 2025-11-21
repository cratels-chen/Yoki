#ifndef F1729055_B7A2_4489_801D_E8E070E34152
#define F1729055_B7A2_4489_801D_E8E070E34152

#include "CheckerBase.h"
#include "CheckerUtils.h"

class MISRA_CPP2023_Rule_9_6_1 : public CheckerBase {
public:
  __CHECKER_CTOR__(MISRA_CPP2023_Rule_9_6_1);

  bool VisitGotoStmt(GotoStmt *node, ASTContext *context) override;
};

#endif /* F1729055_B7A2_4489_801D_E8E070E34152 */
