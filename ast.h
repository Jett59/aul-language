#ifndef AUL_AST_H
#define AUL_AST_H

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

namespace aul {
class AstVisitor {
public:
  virtual ~AstVisitor() {}
};

class AstNode {
public:
  virtual ~AstNode() {}

  virtual void apply(AstVisitor &visitor) = 0;
};
class DefinitionsNode : public AstNode {
public:
  void add(std::unique_ptr<AstNode> node) {
    definitions.push_back(std::move(node));
  }

  virtual void apply(AstVisitor &visitor) {
    std::for_each(
        definitions.begin(), definitions.end(),
        [&visitor](std::unique_ptr<AstNode> &node) { node->apply(visitor); });
  }

private:
  std::vector<std::unique_ptr<AstNode>> definitions;
};
} // namespace aul

#endif