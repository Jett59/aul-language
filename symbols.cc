#include "symbols.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>

int buildSymbolTable(astNode **nodePointer,
                     astNode *previousSymbolTable) {
  astNode *node = *nodePointer;
  if (node->nodeType == symbolTable) {
    return 0;
  } else {
    astNode *nextSymbolTable = previousSymbolTable;
    if (node->nodeType == program) {
      astNode *package = node->children[1];
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
        astNode *child = node->children[i];
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
        astNode *child = node->children[i];
        if (child->nodeType == variableDefinition) {
          addAstNode(&nextSymbolTable,
                     createAstNode(child->fileName, child->line, child->column,
                                   child->nodeType, child->value, child->type,
                                   child->flags, 0));
        }
      }
      addAstNode(&nextSymbolTable, previousSymbolTable);
    } else if (node->nodeType == functionDefinition) {
      astNode *arguments = node->children[0];
      nextSymbolTable = createAstNode(node->fileName, -1, -1, symbolTable,
                                      (union astNodeValue){}, 0, flag_null, 0);
      for (int i = 0; i < arguments->numChildren; i++) {
        astNode *arg = arguments->children[i];
        addAstNode(&nextSymbolTable,
                   createAstNode(arg->fileName, arg->line, arg->column,
                                 arg->nodeType, arg->value, arg->type,
                                 arg->flags, 0));
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

astNode *findSymbol(astNode *symbolTable,
                           const char *identifier) {
  if (symbolTable == nullptr) {
    return nullptr;
  }
  for (int i = 0; i < symbolTable->numChildren - 1; i++) {
    astNode *symbol = symbolTable->children[i];
    if (strcmp(symbol->value.string, identifier) == 0) {
      return symbol;
    }
  }
  return findSymbol(symbolTable->children[symbolTable->numChildren - 1],
                    identifier);
}
