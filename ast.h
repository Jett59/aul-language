#ifndef AUL_AST_H
#define AUL_AST_H

#include "type.h"
#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace aul {
class AstVisitor {
public:
  virtual ~AstVisitor() {}

  virtual std::unique_ptr<AstVisitor> visitDefinitions() { return nullptr; }
  virtual std::unique_ptr<AstVisitor> visitDefinition(bool constant,
                                                      const std::string &name) {
    return nullptr;
  }
  virtual std::unique_ptr<AstVisitor> visitCast(const Type &type) {
    return nullptr;
  }

  virtual void visitInteger(uintmax_t value) {}

  virtual void visitEnd() {}
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
    std::unique_ptr<AstVisitor> definitionsVisitor = visitor.visitDefinitions();
    std::for_each(definitions.begin(), definitions.end(),
                  [&](std::unique_ptr<AstNode> &node) {
                    node->apply(*definitionsVisitor);
                  });
    definitionsVisitor->visitEnd();
  }

private:
  std::vector<std::unique_ptr<AstNode>> definitions;
};
class DefinitionNode : public AstNode {
public:
  DefinitionNode(bool constant, std::string name,
                 std::unique_ptr<AstNode> value)
      : constant(constant), name(std::move(name)), value(std::move(value)) {}

  virtual void apply(AstVisitor &visitor) {
    std::unique_ptr<AstVisitor> valueVisitor =
        visitor.visitDefinition(constant, name);
    value->apply(*valueVisitor);
    valueVisitor->visitEnd();
  }

private:
  bool constant;
  std::string name;
  std::unique_ptr<AstNode> value;
};
class IntegerNode : public AstNode {
public:
  IntegerNode(uintmax_t value) : value(value) {}

  virtual void apply(AstVisitor &visitor) { visitor.visitInteger(value); }

private:
  uintmax_t value;
};
class CastNode : public AstNode {
public:
  CastNode(std::unique_ptr<Type> type, std::unique_ptr<AstNode> value)
      : type(std::move(type)), value(std::move(value)) {}

  virtual void apply(AstVisitor &visitor) {
    std::unique_ptr<AstVisitor> valueVisitor = visitor.visitCast(*type);
    value->apply(*valueVisitor);
  }

private:
  std::unique_ptr<Type> type;
  std::unique_ptr<AstNode> value;
};
} // namespace aul

#endif