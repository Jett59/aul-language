#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct typeNode *createTypeNode(enum typeType type, char *identifier,
                                struct typeNode *child) {
  struct typeNode *node = malloc(sizeof(struct typeNode));
  if (node != 0) {
    node->type = type;
    node->identifier = identifier;
    node->child = child;
  }
  return node;
}

void printType(struct typeNode *node) {
  if (node != 0) {
    enum typeType type = node->type;
    if (type == TYPE_NONE) {
      printf("<none>");
    } else if (type == TYPE_BOOLEAN) {
      printf("boolean");
    } else if (type == TYPE_CHAR) {
      printf("char");
    } else if (type >= TYPE_I8 && type <= TYPE_I64) {
      printf("i%d", 8 << (type - TYPE_I8));
    } else if (type == TYPE_ISIZE) {
      printf("isize");
    } else if (type >= TYPE_U8 && type <= TYPE_U64) {
      printf("u%d", 8 << (type - TYPE_U8));
    } else if (type == TYPE_USIZE) {
      printf("usize");
    } else if (type == TYPE_F32) {
      printf("f32");
    } else if (type == TYPE_F64) {
      printf("f64");
    } else if (type == TYPE_UNKNOWN) {
      printf("unknown");
    } else if (type == TYPE_POINTER) {
      printf("pointer");
    } else {
      fprintf(stderr, "unknown: %d", type);
    }
    if (node->identifier != 0) {
      printf(": %s", node->identifier);
    }
    if (node->child != 0) {
      putchar('(');
      printType(node->child);
      putchar(')');
    }
  } else {
    printf("<null>");
  }
  putchar(' ');
}

int typecmp(struct typeNode *a, struct typeNode *b) {
  return a == b || (a->type == b->type &&
                    (a->identifier == b->identifier ||
                     strcmp(a->identifier, b->identifier) == 0) &&
                    typecmp(a->child, b->child) == 0) ? 0 : 1;
}
