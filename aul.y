%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char* str);
int yylex();

%}

%token identifier

%start code

%%

code: identifier

%%

void yyerror(const char* message) {
    fprintf(stderr, "%s\n", message);
}
