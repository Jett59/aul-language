%{

#define YYERROR_VERBOSE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char* str);
int yylex();

%}

%union {
    const char* string;
    double number;
}

%token identifier number

%type <string> identifier
%type <number> number

%start code

%%

code: number  {printf("found '%f'!\n", $1); };

%%

void yyerror(const char* message) {
    fprintf(stderr, "%s\n", message);
}

int main () {
    return yyparse();
}