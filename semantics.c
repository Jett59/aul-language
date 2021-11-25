#include "semantics.h"
#include "ast.h"
#include "error.h"
#include "symbols.h"
#include "type.h"
#include <stdbool.h>


static bool semanticAssert(bool expression, struct astNode *node,
                           const char *message) {
  if (!expression) {
    error(node->fileName, node->line, node->column, message);
    return false;
  } else {
    return true;
  }
}

static bool canConvertImmediate(struct typeNode *from, struct typeNode *to) {
  if (from->type >= TYPE_I8 && from->type <= TYPE_USIZE) {
    return to->type >= TYPE_I8 && to->type <= TYPE_USIZE;
  } else if (from->type == TYPE_F32 && to->type == TYPE_F32) {
    return true;
  } else if (from->type == TYPE_F64 && to->type == TYPE_F64) {
    return true;
  } else {
    return typecmp(from, to) == 0;
  }
}

static int checkTypes(struct astNode *node) {
  if (node->type == 0) {
    switch (node->nodeType) {
    case moduleDeclaration:
    case packageDefinition:
    case definitions:
    case statements: {
      break; // No type
    }
    case variableDefinition: {
      struct astNode *expression = node->children[0];
      if (expression->nodeType == numberExpression) {
        if (!semanticAssert(canConvertImmediate(expression->type, node->type),
                            expression,
                            "Immediate value of variable is not compatible "
                            "with variable type")) {
          return 1;
        }
      } else {
        if (!semanticAssert(typecmp(node->type, expression->type) == 0,
                            expression,
                            "Error: Type of expression does not match type of "
                            "variable")) {
          return 1;
        }
      }
      break;
    }
    case variableReferenceExpression: {
      struct astNode *variableSymbol =
          findSymbol(node->children[0], node->value.string);
          if (!semanticAssert(variableSymbol != 0, node, "Referenced variable is not visible in this scope")) {
            return 1;
          }else if (!semanticAssert(variableSymbol->nodeType == variableDefinition, node, "Variable reference to non-variable")) {
            return 1;
          }else {
            node->type = variableSymbol->type;
          }
          break;
    }
    case addExpression:
    case subtractExpression:
    case multiplyExpression:
    case divideExpression: {
      if (!semanticAssert(
              typecmp(node->children[0]->type, node->children[1]->type) == 0,
              node, "Binary operation with parameters of different types")) {
        return 1;
      } else {
        node->type = node->children[0]->type;
      }
      break;
    }
    case returnStatement: {
      node->type = node->children[0]->type;
      break;
    }
    }
  }
  return 0;
}

int analyseSemantics(struct astNode *node) {
  if (node->nodeType == symbolTable) {
    return 0;
  }
  for (int i = 0; i < node->numChildren; i++) {
    if (analyseSemantics(node->children[i]) != 0) {
      return 1;
    }
  }
  if (checkTypes(node) != 0) {
    return 1;
  } else {
    return 0;
  }
}
