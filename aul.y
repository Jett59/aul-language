%{

#define YYERROR_VERBOSE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

void yyerror(const char* str);
int yylex();

static struct astNode* astRoot;

%}

%union {
    const char* string;
    double number;
    struct astNode* node;
}

%token identifier number
%token leftParen rightParen leftBracket rightBracket leftBrace rightBrace comma dot semicolon

%type <string> identifier
%type <number> number

%type <node> program

%start program

%%

program: number  {
    $$ = createAstNode(program, (union astNodeValue) {.number = $1}, 0);
    astRoot = $$;
};

%%

void yyerror(const char* message) {
    fprintf(stderr, "%s\n", message);
}

int main () {
    int parseResult = yyparse();
    if (parseResult != 0) {
        yyerror("Something happened!");
        return parseResult;
    }else {
        printf("Parsed with program value of %f\n", astRoot->value.number);
    }
}
