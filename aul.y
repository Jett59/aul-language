%code requires {
    #include "ast.h"
}

%{

#define YYERROR_VERBOSE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char* str);
int yylex();

static struct astNode* astRoot;

%}

%union {
    char* string;
    double number;
    enum astNodeFlags flags;
    struct astNode* node;
}

%token identifier number
%token leftParen rightParen leftBracket rightBracket leftBrace rightBrace comma dot semicolon
%token module package
%token export internal

%type <string> identifier dottedIdentifier
%type <number> number

%type <node> program moduleDeclaration packageDefinition
%type <flags> visibility

%start program

%%

program: moduleDeclaration packageDefinition {
    $$ = createAstNode(program, (union astNodeValue) {}, flag_null, 2, $1, $2);
    astRoot = $$;
};

moduleDeclaration: module dottedIdentifier semicolon {
    $$ = createAstNode(moduleDeclaration, (union astNodeValue) {.string = $2}, flag_null, 0);
}

packageDefinition: visibility package identifier leftBrace rightBrace {
    $$ = createAstNode(packageDefinition, (union astNodeValue) {.string = $3}, $1, 0);
}

visibility: export {
    $$ = flag_export;
}
| internal {
    $$ = flag_internal;
    }
| {
    $$ = flag_internal;
}

dottedIdentifier: dottedIdentifier dot identifier {
    char* newStr = malloc(strlen($1) + strlen($3) + 2);
    strcpy(newStr, $1);
    char dotChar = '.';
    strncat(newStr, &dotChar, 1);
    strcat(newStr, $3);
    free($1);
    free($3);
    $$ = newStr;
}
| identifier;

%%

void yyerror(const char* message) {
    fprintf(stderr, "%s\n", message);
}

int main () {
    int parseResult = yyparse();
    if (parseResult != 0) {
        fprintf(stderr, "Failed to compile.\n");
        return parseResult;
    }else {
        dumpTree(astRoot);
    }
}
