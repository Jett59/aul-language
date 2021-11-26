#include "ast.h"
#include "type.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct astNode *createAstNode(const char *fileName, int line, int column,
                              enum astNodeType nodeType,
                              union astNodeValue value, struct typeNode *type,
                              enum astNodeFlags flags, int numChildren, ...) {
  va_list children;
  va_start(children, numChildren);
  struct astNode *node =
      malloc(sizeof(struct astNode) + numChildren * sizeof(struct astNode *));
  if (node == 0) {
    va_end(children);
    return 0;
  } else {
    node->fileName = fileName;
    node->line = line;
    node->column = column;
    node->nodeType = nodeType;
    node->value = value;
    node->type = type;
    node->flags = flags;
    node->numChildren = numChildren;
    for (int i = 0; i < numChildren; i++) {
      struct astNode* child = va_arg(children, struct astNode *);
      if (child != 0) {
        child->parent = node;
      }
      node->children[i] = child;
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
  for (int i = 0; i < newAstNode->numChildren; i ++) {
    struct astNode *child = newAstNode->children[i];
    child->parent = newAstNode;
  }
  return newAstNode;
}

void printTree(struct astNode *root) {
  if (root == 0) {
    printf("(null)");
  } else {
    printf("%s:%d:%d: ", root->fileName, root->line, root->column);
    switch (root->nodeType) {
    case symbolTable: {
      printf("Symbol table");
      break;
    }
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
      printf("Variable '%s'", root->value.string);
      break;
    }
    case functionDefinition: {
      printf("Function '%s'", root->value.string);
      break;
    }
    case argumentList: {
      printf("Argument list");
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
    case castExpression: {
      printf("Cast");
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
    case multiplyExpression: {
      printf("Multiply");
      break;
    }
    case divideExpression: {
      printf("Divide");
      break;
    }
    default:
      printf("Unknown node type %d", root->nodeType);
      break;
    }
    if (root->numChildren > 0 || root->flags != 0 || root->type != 0) {
      printf(": ");
    }
    if (root->type != 0) {
      printType(root->type);
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
        printTree(root->children[i]);
      }
      printf("}");
    }
  }
  printf("\n");
}
