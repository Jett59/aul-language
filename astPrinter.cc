#include "astPrinter.h"
#include <iostream>

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

void AstPrinter::visitInteger(uintmax_t value) {
  out << "Integer: " << value << std::endl;
}

void AstPrinter::visitEnd() { out << "}" << std::endl; }
} // namespace aul
