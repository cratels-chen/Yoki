#ifndef C2F9D896_D044_4D91_8D6A_DD4BD6DE3ED6
#define C2F9D896_D044_4D91_8D6A_DD4BD6DE3ED6

#include "CheckerUtils.h"
#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>

using namespace clang;

class CheckerBase {
public:
  CheckerBase(std::string name, std::string description,
              CheckerSeverity category)
      : name(name), description(description), category(category) {}

#define __YOKI_VISIT_NODE__(NODE)                                              \
  virtual bool Visit##NODE(NODE *node, ASTContext *context);
#include "visit_node.inc"
#undef __YOKI_VISIT_NODE__

public:
  void setName(const std::string &name) { this->name = name; }
  void setDescription(const std::string &description) {
    this->description = description;
  }
  void setCategory(CheckerSeverity category) { this->category = category; }
  const std::string &getName() const { return name; }
  const std::string &getDescription() const { return description; }
  CheckerSeverity getCategory() const { return category; }

private:
  std::string name;
  std::string description;
  CheckerSeverity category;
};
#endif /* C2F9D896_D044_4D91_8D6A_DD4BD6DE3ED6 */
