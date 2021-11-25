#include "symbols.h"
#include <stdio.h>
#include "ast.h"

int buildSymbolTable(struct astNode **nodePointer,
                     struct astNode *previousSymbolTable) {
  struct astNode *node = *nodePointer;
  if (node->nodeType == symbolTable) {
    return 0;
  } else {
    struct astNode *nextSymbolTable = previousSymbolTable;
    if (node->nodeType == program) {
      struct astNode *package = node->children[1];
      nextSymbolTable = createAstNode(-1, -1,
          symbolTable, (union astNodeValue){}, 0, flag_null, 2,
          createAstNode(package->line, package->column, package->nodeType, package->value, 0, package->flags, 0),
          previousSymbolTable);
    } else if (node->nodeType == definitions) {
      nextSymbolTable =
          createAstNode(-1, -1, symbolTable, (union astNodeValue){}, 0, flag_null, 0);
      for (int i = 0; i < node->numChildren; i++) {
        struct astNode *child = node->children[i];
        addAstNode(&nextSymbolTable,
                   createAstNode(child->line, child->column, child->nodeType, child->value, child->type,
                                 child->flags, 0));
      }
      addAstNode(&nextSymbolTable, previousSymbolTable);
    } else if (node->nodeType == statements) {
      nextSymbolTable =
          createAstNode(-1, -1, symbolTable, (union astNodeValue){}, 0, flag_null, 0);
      for (int i = 0; i < node->numChildren; i++) {
        struct astNode *child = node->children[i];
        if (child->nodeType == variableDefinition) {
          addAstNode(&nextSymbolTable,
                     createAstNode(child->line, child->column, child->nodeType, child->value, child->type,
                                   child->flags, 0));
        }
      }
      addAstNode(&nextSymbolTable, previousSymbolTable);
    }
    for (int i = 0; i < node->numChildren; i++) {
      if (buildSymbolTable(&node->children[i], nextSymbolTable) != 0) {
        return 1;
      }
    }
      addAstNode(nodePointer, nextSymbolTable);
      return 0;
  }
}
