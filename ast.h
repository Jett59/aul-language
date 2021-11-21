#ifndef AUL_AST_H
#define AUL_AST_H

enum astNodeType { program, moduleDeclaration, packageDefinition };

union astNodeValue {
  const char *string;
  double number;
  const char *stringPair[2];
  double numberPair[2];
};

enum astNodeFlags { flag_null = 0, flag_exported = 1, flag_internal = 2 };

struct astNode {
  enum astNodeType type;
  union astNodeValue value;
  enum astNodeFlags flags;
  int numChildren;
  struct astNode *children[0];
};

struct astNode *createAstNode(enum astNodeType type, union astNodeValue value,
                              enum astNodeFlags flags, int numChildren, ...);

void dumpTree(struct astNode *root);

#endif