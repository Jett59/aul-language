#ifndef AUL_AST_H
#define AUL_AST_H

#include "type.h"

enum astNodeType {
  symbolTable, program, moduleDeclaration, packageDefinition, definitions,
  variableDefinition, functionDefinition, argumentList, statements,
  returnStatement, numberExpression, variableReferenceExpression,
  castExpression, assignExpression, addExpression, subtractExpression,
  multiplyExpression, divideExpression
};

union astNodeValue {
  const char *string;
  double number;
};

enum astNodeFlags {
  flag_null = 0,
  flag_export = 1,
  flag_internal = 2,
  flag_instance = 4,
  flag_static = 8
};

static inline astNodeFlags operator|(astNodeFlags a, astNodeFlags b) {
      return static_cast<astNodeFlags>(static_cast<int>(a) |
                                       static_cast<int>(b));
                                       }

struct astNode {
  const char *fileName;
  int line;
  int column;
  astNode *parent;
  astNodeType nodeType;
  astNodeValue value;
  typeNode *type;
  astNodeFlags flags;
  int numChildren;
  astNode *children[0];
};

astNode *createAstNode(const char *fileName, int line, int column,
                              astNodeType nodeType,
                              astNodeValue value, typeNode *type,
                              astNodeFlags flags, int numChildren, ...);

astNode *addAstNode(astNode **dest, astNode *element);

void printTree(astNode *root);

#endif