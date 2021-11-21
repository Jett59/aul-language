%{

#include <stdlib.h>
#include "aul.tab.h"

void yyerror(const char* message);

void storeString(void);
void storeNumber(void);

%}

%%

"//".*   ;

"\n"|" "|"\t"   ;

"module"  return module;
"package"  return package;

[a-zA-Z\_][a-zA-Z\_0-9]*   storeString(); return identifier;
[0-9]+"."?[0-9]*   storeNumber(); return number;

"("  return leftParen;
")"  return rightParen;
"["  return leftBracket;
"]"  return rightBracket;
"{"  return leftBrace;
"}"  return rightBrace;
","  return comma;
"."  return dot;
";"  return semicolon;

.   yyerror("Unknown character"); return 0;

%%

void storeString(void) {
    char* newString = malloc(strlen(yytext) + 1);
    strcpy(newString, yytext);
    yylval.string = newString;
}

void storeNumber(void) {
    yylval.number = atof(yytext);
}
