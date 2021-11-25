#ifndef AUL_AST_H
#define AUL_AST_H

#include "type.h"

enum astNodeType {
  symbolTable,
  program,
  moduleDeclaration,
  packageDefinition,
  definitions,
  variableDefinition,
  functionDefinition,
  statements,
  returnStatement,
  numberExpression,
  variableReferenceExpression,
  assignExpression,
  addExpression,
  subtractExpression,
  multiplyExpression,
  divideExpression
};

union astNodeValue {
  const char *string;
  double number;
};

enum astNodeFlags { flag_null = 0, flag_export = 1, flag_internal = 2, flag_instance = 4, flag_static = 8 };

struct astNode {
  int line;
  int column;
  enum astNodeType nodeType;
  union astNodeValue value;
  struct typeNode *type;
  enum astNodeFlags flags;
  int numChildren;
  struct astNode *children[0];
};

struct astNode *createAstNode(int line, int column, enum astNodeType nodeType, union astNodeValue value,
                              struct typeNode* type, enum astNodeFlags flags, int numChildren, ...);

struct astNode *addAstNode(struct astNode **dest,
                           struct astNode *element);

void printTree(struct astNode *root);

#endif