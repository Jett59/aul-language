#ifndef AUL_AST_H
#define AUL_AST_H

enum astNodeType {
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
  addExpression,
  subtractExpression
};

union astNodeValue {
  const char *string;
  double number;
  const char *stringPair[2];
  double numberPair[2];
};

enum astNodeFlags { flag_null = 0, flag_export = 1, flag_internal = 2, flag_instance = 4, flag_static = 8 };

struct astNode {
  enum astNodeType type;
  union astNodeValue value;
  enum astNodeFlags flags;
  int numChildren;
  struct astNode *children[0];
};

struct astNode *createAstNode(enum astNodeType type, union astNodeValue value,
                              enum astNodeFlags flags, int numChildren, ...);

struct astNode *addAstNode(struct astNode **dest,
                           struct astNode *element);

void dumpTree(struct astNode *root);

#endif