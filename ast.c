#include "ast.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct astNode *createAstNode(enum astNodeType type, union astNodeValue value,
                              int numChildren, ...) {
  va_list children;
  va_start(children, numChildren);
  struct astNode *node =
      malloc(sizeof(struct astNode) + numChildren * sizeof(struct astNode *));
  if (node == 0) {
    va_end(children);
    return 0;
  } else {
    node->type = type;
    node->value = value;
    node->numChildren = numChildren;
    for (int i = 0; i < numChildren; i++) {
      node->children[i] = va_arg(children, struct astNode *);
    }
    return node;
  }
}

void dumpTree(struct astNode *root) {
  switch (root->type) {
  case program: {
    if (root->numChildren != 2) {
      fprintf(stderr,
              "Warning: wrong number of children for type program: %d\n",
              root->numChildren);
    }
    printf("Program ");
    break;
  }
  case moduleDeclaration: {
    printf("module %s ", root->value.string);
    break;
  }
  case packageDefinition: {
    printf("package %s ", root->value.string);
    break;
  }
  default:
    fprintf(stderr, "Warning: unknown node type %d\n", root->type);
    break;
  }
  printf("{\n");
  for (int i = 0; i < root->numChildren; i++) {
    dumpTree(root->children[i]);
  }
  printf("}\n");
}
