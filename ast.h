#ifndef AUL_AST_H
#define AUL_AST_H

enum astNodeType {program, moduleDeclaration, packageDefinition};

union astNodeValue {
  const char* string;
  double number;
};

struct astNode {
  enum astNodeType type;
  int numChildren;
  union astNodeValue value;
  struct astNode* children[0];
};

struct astNode* createAstNode(enum astNodeType type, union astNodeValue value,
                       int numChildren, ...);

void dumpTree(struct astNode* root);

#endif