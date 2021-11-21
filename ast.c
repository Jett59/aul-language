#include "ast.h"
#include <stdarg.h>
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
