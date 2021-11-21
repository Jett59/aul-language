#include "ast.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct astNode *createAstNode(enum astNodeType type, union astNodeValue value,
                              enum astNodeFlags flags, int numChildren, ...) {
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
    node->flags = flags;
    node->numChildren = numChildren;
    for (int i = 0; i < numChildren; i++) {
      node->children[i] = va_arg(children, struct astNode *);
    }
    return node;
  }
}

struct astNode *addAstNode(struct astNode **dest, struct astNode *element) {
  int newElementCount = (*dest)->numChildren + 1;
  struct astNode *newAstNode = *dest =
      realloc(*dest, sizeof(struct astNode) +
                         newElementCount * sizeof(struct astNode *));
  newAstNode->children[newAstNode->numChildren++] = element;
  return newAstNode;
}

void dumpTree(struct astNode *root) {
  switch (root->type) {
  case program: {
    if (root->numChildren != 2) {
      fprintf(stderr,
              "Warning: wrong number of children for type program: %d\n",
              root->numChildren);
    }
    printf("Program: ");
    break;
  }
  case moduleDeclaration: {
    printf("module %s: ", root->value.string);
    break;
  }
  case packageDefinition: {
    printf("package %s: ", root->value.string);
    break;
  }
  case definitions: {
    printf("definitions: ");
    break;
  }
  case functionDefinition: {
    printf("Function '%s' returns '%s': ", root->value.stringPair[1],
           root->value.stringPair[0]);
    break;
  }
  default:
    fprintf(stderr, "Warning: unknown node type %d\n", root->type);
    break;
  }
  enum astNodeFlags flags = root->flags;
  if (flags & flag_export) {
    printf("export ");
  } else if (flags & flag_internal) {
    printf("internal ");
  }
  if (flags & flag_static) {
    printf("static ");
  } else if (flags & flag_instance) {
    printf("instance ");
  }
  printf("{\n");
  for (int i = 0; i < root->numChildren; i++) {
    dumpTree(root->children[i]);
  }
  printf("}\n");
}
