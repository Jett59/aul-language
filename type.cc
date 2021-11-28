#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace aul {
typeNode *createTypeNode(typeType type, char *identifier,
                                typeNode *child) {
  typeNode *node = (typeNode*)malloc(sizeof(typeNode));
  if (node != nullptr) {
    node->type = type;
    node->identifier = identifier;
    node->child = child;
  }
  return node;
}

void printType(typeNode *node) {
  if (node != nullptr) {
    typeType type = node->type;
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

int typecmp(typeNode *a, typeNode *b) {
  if (a == nullptr) {
    if (b == nullptr) {
      return 0;
    } else {
      return 1;
    }
  } else {
    return (a == b || a == 0 || b == 0 ||
            (a->type == b->type &&
             (a->identifier == b->identifier ||
              strcmp(a->identifier, b->identifier) == 0) &&
             typecmp(a->child, b->child) == 0))
               ? 0
               : 1;
  }
}
}
