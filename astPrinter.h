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
                                                      const std::string &name) {
    out << "Definition: " << (constant ? "constant" : "mutable") << " " << name
        << " {" << std::endl;
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