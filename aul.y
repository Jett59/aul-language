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

%token IDENTIFIER NUMBER
%token LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE EQUALS COMMA DOT SEMICOLON
%token MODULE PACKAGE
%token EXPORT INTERNAL
%token INSTANCE STATIC

%type <string> IDENTIFIER dottedIdentifier
%type <number> NUMBER

%type <node> program moduleDeclaration packageDefinition definitions definition variableDefinition functionDefinition expression
%type <flags> visibility scope

%start program

%%

program: moduleDeclaration packageDefinition {
    $$ = createAstNode(program, (union astNodeValue) {}, flag_null, 2, $1, $2);
    astRoot = $$;
};

moduleDeclaration: MODULE dottedIdentifier SEMICOLON {
    $$ = createAstNode(moduleDeclaration, (union astNodeValue) {.string = $2}, flag_null, 0);
}

packageDefinition: visibility PACKAGE IDENTIFIER LEFT_BRACE definitions RIGHT_BRACE {
    $$ = createAstNode(packageDefinition, (union astNodeValue) {.string = $3}, $1, 1, $5);
}

definitions: definitions definition {
    $$ = addAstNode(&$1, $2);
}
| definition {
    $$ = createAstNode(definitions, (union astNodeValue) {}, flag_null, 1, $1);
}

definition: variableDefinition | functionDefinition;

variableDefinition: visibility scope IDENTIFIER IDENTIFIER EQUALS expression SEMICOLON {
    $$ = createAstNode(variableDefinition, (union astNodeValue) {.stringPair = {$3, $4}}, $1 | $2, 1, $6);
}

functionDefinition: visibility scope IDENTIFIER IDENTIFIER LEFT_PAREN RIGHT_PAREN LEFT_BRACE RIGHT_BRACE {
    $$ = createAstNode(functionDefinition, (union astNodeValue) {.stringPair = {$3, $4}}, $1 | $2, 0);
}

expression: NUMBER {
    $$ = createAstNode(numberExpression, (union astNodeValue) {.number = $1}, flag_null, 0);
}

visibility: EXPORT {
    $$ = flag_export;
}
| INTERNAL {
    $$ = flag_internal;
    }
| {
    $$ = flag_internal;
}

scope: STATIC {
    $$ = flag_static;
}
| INSTANCE {
    $$ = flag_instance;
}
| {
    $$ = flag_instance;
}

dottedIdentifier: dottedIdentifier DOT IDENTIFIER {
    char* newStr = malloc(strlen($1) + strlen($3) + 2);
    strcpy(newStr, $1);
    char dotChar = '.';
    strncat(newStr, &dotChar, 1);
    strcat(newStr, $3);
    free($1);
    free($3);
    $$ = newStr;
}
| IDENTIFIER;

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
