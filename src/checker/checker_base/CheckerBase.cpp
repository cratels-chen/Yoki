#include "CheckerBase.h"

#define __YOKI_VISIT_NODE__(NODE)                                              \
  bool CheckerBase::Visit##NODE(NODE *node, ASTContext *context) {             \
    return true;                                                               \
  }

#include "visit_node.inc"
#undef __YOKI_VISIT_NODE__
