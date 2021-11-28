#ifndef AUL_SYMBOLS_H
#define AUL_SYMBOLS_H  

#include "ast.h"

namespace aul {
int buildSymbolTable(astNode** node, astNode* previousSymbolTable);

astNode* findSymbol(astNode* symbolTable, const char* identifier);
}

#endif