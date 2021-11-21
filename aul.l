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

"export"  return EXPORT;
"internal"  return INTERNAL;
"static"  return STATIC;
"instance"  return INSTANCE;
"module"  return MODULE;
"package"  return PACKAGE;
"return"  return RETURN;

[a-zA-Z\_][a-zA-Z\_0-9]*   storeString(); return IDENTIFIER;
[0-9]+"."?[0-9]*   storeNumber(); return NUMBER;

"("  return LEFT_PAREN;
")"  return RIGHT_PAREN;
"["  return LEFT_BRACKET;
"]"  return RIGHT_BRACKET;
"{"  return LEFT_BRACE;
"}"  return RIGHT_BRACE;
"="  return EQUALS;
","  return COMMA;
"."  return DOT;
";"  return SEMICOLON;

"+"  return PLUS;
"-"  return MINUS;

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
