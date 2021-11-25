#ifndef AUL_SYMBOLS_H
#define AUL_SYMBOLS_H  

#include "ast.h"

int buildSymbolTable(struct astNode** node, struct astNode* previousSymbolTable);

struct astNode* findSymbol(struct astNode* symbolTable, const char* identifier);

#endif