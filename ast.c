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
    printf("Program");
    break;
  }
  case moduleDeclaration: {
    printf("module '%s'", root->value.string);
    break;
  }
  case packageDefinition: {
    printf("package '%s'", root->value.string);
    break;
  }
  case definitions: {
    printf("definitions");
    break;
  }
  case statements: {
    printf("Statements");
    break;
  }
  case variableDefinition: {
    printf("Variable '%s' of type '%s'", root->value.stringPair[1],
           root->value.stringPair[0]);
    break;
  }
  case functionDefinition: {
    printf("Function '%s' returns '%s'", root->value.stringPair[1],
           root->value.stringPair[0]);
    break;
  }
  case returnStatement: {
    printf("Return");
    break;
  }
  case numberExpression: {
    printf("Number %f", root->value.number);
    break;
  }
  case variableReferenceExpression: {
    printf("Variable reference '%s'", root->value.string);
    break;
  }
  case assignExpression: {
      printf("Assign");
      break;
  }
  case addExpression: {
      printf("Add");
      break;
  }
  case subtractExpression: {
    printf("Subtract");
    break;
  }
  default:
    printf("Unknown node type %d", root->type);
    break;
  }
  if (root->numChildren > 0 || root->flags != 0) {
    printf(": ");
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
  if (root->numChildren > 0) {
  printf("{\n");
  for (int i = 0; i < root->numChildren; i++) {
    dumpTree(root->children[i]);
  }
  printf("}");
  }
  printf("\n");
}
