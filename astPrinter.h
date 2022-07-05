#ifndef AUL_AST_PRINTER_H
#define AUL_AST_PRINTER_H

#include "ast.h"
#include <iostream>
#include <memory>

namespace aul {
class AstPrinter : public AstVisitor {
public:
  AstPrinter(std::ostream &out) : out(out) {}

  virtual std::unique_ptr<AstVisitor> visitDefinitions() {
    out << "Definitions {" << std::endl;
    return std::make_unique<AstPrinter>(out);
  }

  virtual std::unique_ptr<AstVisitor> visitDefinition(bool constant,
                                                      const std::string &name) override {
    out << "Definition: " << (constant ? "constant" : "mutable") << " " << name
        << " {" << std::endl;
    return std::make_unique<AstPrinter>(out);
  }

virtual std::unique_ptr<AstVisitor> visitCast(const Type &type) override {
  out << "Cast<" << type.toString() << "> {" << std::endl;
  return std::make_unique<AstPrinter>(out);
}
virtual std::unique_ptr<AstVisitor> visitFunction(const std::vector<NamedType> &parameterTypes) override {
  out << "Function:" << std::endl;
  std::for_each(parameterTypes.begin(), parameterTypes.end(),
                [&](const NamedType &type) {
                  out << "Parameter: " << type.name << ": " << type.type->toString() << std::endl;
                });
  out << "{" << std::endl;
  return std::make_unique<AstPrinter>(out);
}

virtual void visitInteger(uintmax_t value) {
  out << "Integer: " << value << std::endl;
  }

  virtual void visitEnd() { out << "}" << std::endl; }

private:
  std::ostream &out;
};
} // namespace aul

#endif