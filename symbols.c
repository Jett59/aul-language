#include "symbols.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>

int buildSymbolTable(struct astNode **nodePointer,
                     struct astNode *previousSymbolTable) {
  struct astNode *node = *nodePointer;
  if (node->nodeType == symbolTable) {
    return 0;
  } else {
    struct astNode *nextSymbolTable = previousSymbolTable;
    if (node->nodeType == program) {
      struct astNode *package = node->children[1];
      nextSymbolTable =
          createAstNode(node->fileName, -1, -1, symbolTable,
                        (union astNodeValue){}, 0, flag_null, 2,
                        createAstNode(package->fileName, package->line,
                                      package->column, package->nodeType,
                                      package->value, 0, package->flags, 0),
                        previousSymbolTable);
    } else if (node->nodeType == definitions) {
      nextSymbolTable = createAstNode(node->fileName, -1, -1, symbolTable,
                                      (union astNodeValue){}, 0, flag_null, 0);
      for (int i = 0; i < node->numChildren; i++) {
        struct astNode *child = node->children[i];
        addAstNode(&nextSymbolTable,
                   createAstNode(child->fileName, child->line, child->column,
                                 child->nodeType, child->value, child->type,
                                 child->flags, 0));
      }
      addAstNode(&nextSymbolTable, previousSymbolTable);
    } else if (node->nodeType == statements) {
      nextSymbolTable = createAstNode(node->fileName, -1, -1, symbolTable,
                                      (union astNodeValue){}, 0, flag_null, 0);
      for (int i = 0; i < node->numChildren; i++) {
        struct astNode *child = node->children[i];
        if (child->nodeType == variableDefinition) {
          addAstNode(&nextSymbolTable,
                     createAstNode(child->fileName, child->line, child->column,
                                   child->nodeType, child->value, child->type,
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

struct astNode *findSymbol(struct astNode *symbolTable,
                           const char *identifier) {
  if (symbolTable == 0) {
    return 0;
  }
  for (int i = 0; i < symbolTable->numChildren - 1; i++) {
    struct astNode *symbol = symbolTable->children[i];
    if (strcmp(symbol->value.string, identifier) == 0) {
      return symbol;
    }
  }
  return findSymbol(symbolTable->children[symbolTable->numChildren - 1], identifier);
}
