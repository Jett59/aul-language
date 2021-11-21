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
%token leftParen rightParen leftBracket rightBracket leftBrace rightBrace equals comma dot semicolon
%token module package
%token export internal
%token instanceScope staticScope

%type <string> identifier dottedIdentifier
%type <number> number

%type <node> program moduleDeclaration packageDefinition definitions definition variableDefinition functionDefinition expression
%type <flags> visibility scope

%start program

%%

program: moduleDeclaration packageDefinition {
    $$ = createAstNode(program, (union astNodeValue) {}, flag_null, 2, $1, $2);
    astRoot = $$;
};

moduleDeclaration: module dottedIdentifier semicolon {
    $$ = createAstNode(moduleDeclaration, (union astNodeValue) {.string = $2}, flag_null, 0);
}

packageDefinition: visibility package identifier leftBrace definitions rightBrace {
    $$ = createAstNode(packageDefinition, (union astNodeValue) {.string = $3}, $1, 1, $5);
}

definitions: definitions definition {
    $$ = addAstNode(&$1, $2);
}
| definition {
    $$ = createAstNode(definitions, (union astNodeValue) {}, flag_null, 1, $1);
}

definition: variableDefinition | functionDefinition;

variableDefinition: visibility scope identifier identifier equals expression semicolon {
    $$ = createAstNode(variableDefinition, (union astNodeValue) {.stringPair = {$3, $4}}, $1 | $2, 1, $6);
}

functionDefinition: visibility scope identifier identifier leftParen rightParen leftBrace rightBrace {
    $$ = createAstNode(functionDefinition, (union astNodeValue) {.stringPair = {$3, $4}}, $1 | $2, 0);
}

expression: number {
    $$ = createAstNode(numberExpression, (union astNodeValue) {.number = $1}, flag_null, 0);
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

scope: {
    $$ = flag_instance;
}
| staticScope {
    $$ = flag_static;
}
| instanceScope {
    $$ = flag_instance;
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
