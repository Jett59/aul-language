#include "semantics.h"
#include "ast.h"
#include "error.h"
#include "symbols.h"
#include "type.h"
#include <stdbool.h>

namespace aul {
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

static bool isCompatibleType(struct astNode *a, struct astNode *b) {
  if (a->nodeType == numberExpression) {
    return canConvertImmediate(a->type, b->type);
  } else if (b->nodeType == numberExpression) {
    return canConvertImmediate(b->type, a->type);
  } else {
    return typecmp(a->type, b->type) == 0;
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
      if (!semanticAssert(isCompatibleType(node, expression), expression,
                          "Variable value not compatible with variable type")) {
        return 1;
      }
      break;
    }
    case variableReferenceExpression: {
      struct astNode *variableSymbol =
          findSymbol(node->children[0], node->value.string);
      if (!semanticAssert(variableSymbol != 0, node,
                          "Referenced variable is not visible in this scope")) {
        return 1;
      } else if (!semanticAssert(variableSymbol->nodeType == variableDefinition,
                                 node, "Variable reference to non-variable")) {
        return 1;
      } else {
        node->type = variableSymbol->type;
      }
      break;
    }
    case assignExpression:
    case addExpression:
    case subtractExpression:
    case multiplyExpression:
    case divideExpression: {
      if (!semanticAssert(
              isCompatibleType(node->children[0], node->children[1]), node,
              "Binary operation with parameters of different types")) {
        return 1;
      } else {
        node->type = node->children[0]->type;
      }
      break;
    }
    case returnStatement: {
      node->type = node->children[0]->type;
      struct astNode *parent = node->parent;
      while (parent != 0 && parent->nodeType != functionDefinition) {
        parent = parent->parent;
      }
      if (!semanticAssert(parent != 0, node, "Return statement not in a function")) {
        return 1;
      }
      if (!semanticAssert(
          typecmp(node->type, parent->type) == 0, node,
          "Return statement returns different type to function return type")) {
        return 1;
          }
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
}
