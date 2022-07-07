#include "astPrinter.h"
#include <iostream>
#include <map>

namespace aul {
std::unique_ptr<AstVisitor> AstPrinter::visitDefinitions() {
  out << "Definitions {" << std::endl;
  return std::make_unique<AstPrinter>(out);
}

std::unique_ptr<AstVisitor>
AstPrinter::visitDefinition(bool constant, const std::string &name) {
  out << "Definition: " << (constant ? "constant" : "mutable") << " " << name
      << " {" << std::endl;
  return std::make_unique<AstPrinter>(out);
}

std::unique_ptr<AstVisitor> AstPrinter::visitCast(const Type &type) {
  out << "Cast<" << type.toString() << "> {" << std::endl;
  return std::make_unique<AstPrinter>(out);
}

std::unique_ptr<AstVisitor>
AstPrinter::visitFunction(const std::vector<NamedType> &parameterTypes) {
  out << "Function:" << std::endl;
  std::for_each(parameterTypes.begin(), parameterTypes.end(),
                [&](const NamedType &type) {
                  out << "Parameter: " << type.name << ": "
                      << type.type->toString() << std::endl;
                });
  out << "{" << std::endl;
  return std::make_unique<AstPrinter>(out);
}
std::unique_ptr<AstVisitor> AstPrinter::visitBlock() {
  out << "Block {" << std::endl;
  return std::make_unique<AstPrinter>(out);
}

static const std::map<BinaryOperatorType, std::string>
    binaryOperatorTypeToString = {
        {BinaryOperatorType::ADD, "+"},
        {BinaryOperatorType::SUBTRACT, "-"},
        {BinaryOperatorType::MULTIPLY, "*"},
        {BinaryOperatorType::DIVIDE, "/"},
        {BinaryOperatorType::MODULO, "%"},
        {BinaryOperatorType::BITWISE_XOR, "^"},
        {BinaryOperatorType::BITWISE_AND, "&"},
        {BinaryOperatorType::BITWISE_OR, "|"},
        {BinaryOperatorType::BITWISE_LEFT_SHIFT, "<<"},
        {BinaryOperatorType::BITWISE_RIGHT_SHIFT, ">>"},
        {BinaryOperatorType::ASSIGN, "="},
        {BinaryOperatorType::LESS, "<"},
        {BinaryOperatorType::LESS_EQUAL, "<="},
        {BinaryOperatorType::GREATER, ">"},
        {BinaryOperatorType::GREATER_EQUAL, ">="},
        {BinaryOperatorType::EQUAL, "=="},
        {BinaryOperatorType::NOT_EQUAL, "!="}};

std::unique_ptr<AstVisitor>
AstPrinter::visitBinaryExpression(BinaryOperatorType operatorType) {
  out << binaryOperatorTypeToString.at(operatorType) << " {" << std::endl;
  return std::make_unique<AstPrinter>(out);
}

static const std::map<UnaryOperatorType, std::string> unaryOperatorToString = {
    {UnaryOperatorType::NEGATE, "- (unary)"},
    {UnaryOperatorType::BITWISE_NOT, "~"},
    {UnaryOperatorType::LOGICAL_NOT, "!"},
    {UnaryOperatorType::PREFIX_INCREMENT, "++ (prefix)"},
    {UnaryOperatorType::PREFIX_DECREMENT, "-- (prefix"},
    {UnaryOperatorType::POSTFIX_INCREMENT, "++ (postfix)"},
    {UnaryOperatorType::POSTFIX_DECREMENT, "-- (postfix)"}};

std::unique_ptr<AstVisitor> AstPrinter::visitUnaryExpression(UnaryOperatorType operatorType) {
  out << unaryOperatorToString.at(operatorType) << " {" << std::endl;
  return std::make_unique<AstPrinter>(out);
}

void AstPrinter::visitInteger(uintmax_t value) {
  out << "Integer: " << value << std::endl;
}
void AstPrinter::visitVariableReference(const std::string &name) {
  out << "VariableReference: " << name << std::endl;
}

void AstPrinter::visitEnd() { out << "}" << std::endl; }
} // namespace aul
