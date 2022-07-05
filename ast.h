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
  virtual std::unique_ptr<AstVisitor>
  visitFunction(const std::vector<NamedType> &parameterTypes) {
    return nullptr;
  }
  virtual std::unique_ptr<AstVisitor> visitBlock() { return nullptr; }

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
    if (definitionsVisitor) {
      std::for_each(definitions.begin(), definitions.end(),
                    [&](std::unique_ptr<AstNode> &node) {
                      node->apply(*definitionsVisitor);
                    });
      definitionsVisitor->visitEnd();
    }
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
    if (valueVisitor) {
      value->apply(*valueVisitor);
      valueVisitor->visitEnd();
    }
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
    if (valueVisitor) {
      value->apply(*valueVisitor);
      valueVisitor->visitEnd();
    }
  }

private:
  std::unique_ptr<Type> type;
  std::unique_ptr<AstNode> value;
};
class FunctionNode : public AstNode {
public:
  FunctionNode(std::vector<NamedType> parameterTypes,
               std::vector<std::unique_ptr<AstNode>> body)
      : parameterTypes(std::move(parameterTypes)), body(std::move(body)) {}

  virtual void apply(AstVisitor &visitor) {
    std::unique_ptr<AstVisitor> bodyVisitor =
        visitor.visitFunction(parameterTypes);
    if (bodyVisitor) {
      std::for_each(
          body.begin(), body.end(),
          [&](std::unique_ptr<AstNode> &node) { node->apply(*bodyVisitor); });
      bodyVisitor->visitEnd();
    }
  }

private:
  std::vector<NamedType> parameterTypes;
  std::vector<std::unique_ptr<AstNode>> body;
};
class BlockStatementNode : public AstNode {
public:
  BlockStatementNode(std::vector<std::unique_ptr<AstNode>> statements)
      : statements(std::move(statements)) {}

  virtual void apply(AstVisitor &visitor) {
    std::unique_ptr<AstVisitor> statementsVisitor = visitor.visitBlock();
    if (statementsVisitor) {
      std::for_each(statements.begin(), statements.end(),
                    [&](std::unique_ptr<AstNode> &node) {
                      node->apply(*statementsVisitor);
                    });
    }
  }

private:
  std::vector<std::unique_ptr<AstNode>> statements;
};
} // namespace aul

#endif