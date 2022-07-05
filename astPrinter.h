#ifndef AUL_AST_PRINTER_H
#define AUL_AST_PRINTER_H

#include "ast.h"
#include <iostream>
#include <memory>

namespace aul {
class AstPrinter : public AstVisitor {
public:
  AstPrinter(std::ostream &out) : out(out) {}

  virtual std::unique_ptr<AstVisitor> visitDefinitions() override;

  virtual std::unique_ptr<AstVisitor>
  visitDefinition(bool constant, const std::string &name) override;

  virtual std::unique_ptr<AstVisitor> visitCast(const Type &type) override;

  virtual std::unique_ptr<AstVisitor>
  visitFunction(const std::vector<NamedType> &parameterTypes) override;

  virtual void visitInteger(uintmax_t value) override;

  virtual void visitEnd() override;

private:
  std::ostream &out;
};
} // namespace aul

#endif