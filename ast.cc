#include "ast.h"

namespace aul {
void DefinitionsNode::apply(AstVisitor &visitor) {
  std::unique_ptr<AstVisitor> definitionsVisitor = visitor.visitDefinitions();
  if (definitionsVisitor) {
    std::for_each(definitions.begin(), definitions.end(),
                  [&](std::unique_ptr<AstNode> &node) {
                    node->apply(*definitionsVisitor);
                  });
    definitionsVisitor->visitEnd();
  }
}
void DefinitionNode::apply(AstVisitor &visitor) {
  std::unique_ptr<AstVisitor> valueVisitor =
      visitor.visitDefinition(constant, name);
  if (valueVisitor) {
    value->apply(*valueVisitor);
    valueVisitor->visitEnd();
  }
}
void IntegerNode::apply(AstVisitor &visitor) { visitor.visitInteger(value); }
void CastNode::apply(AstVisitor &visitor) {
  std::unique_ptr<AstVisitor> valueVisitor = visitor.visitCast(*type);
  if (valueVisitor) {
    value->apply(*valueVisitor);
    valueVisitor->visitEnd();
  }
}
void FunctionNode::apply(AstVisitor &visitor) {
  std::unique_ptr<AstVisitor> bodyVisitor =
      visitor.visitFunction(parameterTypes);
  if (bodyVisitor) {
    std::for_each(
        body.begin(), body.end(),
        [&](std::unique_ptr<AstNode> &node) { node->apply(*bodyVisitor); });
    bodyVisitor->visitEnd();
  }
}
void BlockStatementNode::apply(AstVisitor &visitor) {
  std::unique_ptr<AstVisitor> statementsVisitor = visitor.visitBlock();
  if (statementsVisitor) {
    std::for_each(statements.begin(), statements.end(),
                  [&](std::unique_ptr<AstNode> &node) {
                    node->apply(*statementsVisitor);
                  });
    statementsVisitor->visitEnd();
  }
}
void BinaryExpressionNode::apply(AstVisitor &visitor) {
  std::unique_ptr<AstVisitor> operandVisitor =
      visitor.visitBinaryExpression(operatorType);
  if (operandVisitor) {
    left->apply(*operandVisitor);
    right->apply(*operandVisitor);
    operandVisitor->visitEnd();
  }
}
} // namespace aul
