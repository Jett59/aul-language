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

[a-zA-Z\_][a-zA-Z\_0-9]*   storeString(); return identifier;
[0-9]+"."?[0-9]*   storeNumber(); return number;

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
