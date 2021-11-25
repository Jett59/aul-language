%code requires {
    #include "ast.h"
    #include "type.h"
    #include <stdio.h>

    int parse(FILE* input, const char* file, struct astNode** ast);
}

%debug

%locations

%{

#define YYERROR_VERBOSE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

void yyerror(const char* message);
int yylex();

static struct astNode* astRoot;

extern FILE* yyin;

static const char* fileName;

#define buildAstNode(nodeType, value, type, numChildren, ...)  createAstNode(fileName, yyloc.first_line, yyloc.first_column, nodeType, value, type, numChildren, __VA_ARGS__);

%}

%union {
    char* string;
    double number;
    enum astNodeFlags flags;
    struct astNode* node;
    struct typeNode* type;
}

%token IDENTIFIER INTEGER DECIMAL
%token LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE EQUALS COMMA DOT SEMICOLON
%token LESS_THAN GREATER_THAN PLUS MINUS MULTIPLY DIVIDE
%token MODULE PACKAGE
%token EXPORT INTERNAL
%token INSTANCE STATIC
%token RETURN
%token BOOLEAN CHAR I8 I16 I32 I64 ISIZE U8 U16 U32 U64 USIZE F32 F64 PTR
%token TRUE FALSE

%type <string> IDENTIFIER dottedIdentifier
%type <number> INTEGER DECIMAL

%type <type> type
%type <node> program moduleDeclaration packageDefinition definitions definition statement statements localVariableDefinition variableDefinition functionDefinition returnStatement expression
%type <flags> visibility scope

%right EQUALS
%left PLUS MINUS
%left MULTIPLY DIVIDE

%start program

%%

program: moduleDeclaration packageDefinition {
    $$ = buildAstNode(program, (union astNodeValue) {}, 0, flag_null, 2, $1, $2);
    astRoot = $$;
}

moduleDeclaration: MODULE dottedIdentifier SEMICOLON {
    $$ = buildAstNode(moduleDeclaration, (union astNodeValue) {.string = $2}, 0, flag_null, 0);
}

packageDefinition: visibility PACKAGE IDENTIFIER LEFT_BRACE definitions RIGHT_BRACE {
    $$ = buildAstNode(packageDefinition, (union astNodeValue) {.string = $3}, 0, $1, 1, $5);
}

definitions: definitions definition {
    $$ = addAstNode(&$1, $2);
}
| {
    $$ = buildAstNode(definitions, (union astNodeValue) {}, 0, flag_null, 0);
}

definition: variableDefinition | functionDefinition;

statements: statements statement {
    $$ = addAstNode(&$1, $2);
}
| {
    $$ = buildAstNode(statements, (union astNodeValue) {}, 0, flag_null, 0);
}

statement: returnStatement | expression SEMICOLON | localVariableDefinition;

localVariableDefinition: type IDENTIFIER EQUALS expression SEMICOLON {
    $$ = buildAstNode(variableDefinition, (union astNodeValue) {.string = $2}, $1, flag_null, 1, $4);
}

variableDefinition: visibility scope type IDENTIFIER EQUALS expression SEMICOLON {
    $$ = buildAstNode(variableDefinition, (union astNodeValue) {.string = $4}, $3, $1 | $2, 1, $6);
}

functionDefinition: visibility scope type IDENTIFIER LEFT_PAREN RIGHT_PAREN LEFT_BRACE statements RIGHT_BRACE {
    $$ = buildAstNode(functionDefinition, (union astNodeValue) {.string = $4}, $3, $1 | $2, 1, $8);
}
| visibility scope IDENTIFIER LEFT_PAREN RIGHT_PAREN LEFT_BRACE statements RIGHT_BRACE {
    $$ = buildAstNode(functionDefinition, (union astNodeValue) {.string = $3}, createTypeNode(TYPE_NONE, 0, 0), $1 | $2, 1, $7);
}

returnStatement: RETURN expression SEMICOLON {
    $$ = buildAstNode(returnStatement, (union astNodeValue) {}, 0, flag_null, 1, $2);
}

expression: LEFT_PAREN expression RIGHT_PAREN {
    $$ = $2;
}
| DECIMAL {
    $$ = buildAstNode(numberExpression, (union astNodeValue) {.number = $1}, createTypeNode(TYPE_F64, 0, 0), flag_null, 0);
}
| INTEGER {
    $$ = buildAstNode(numberExpression, (union astNodeValue) {.number = $1}, createTypeNode(TYPE_I64, 0, 0), flag_null, 0);
}
| TRUE {
    $$ = buildAstNode(numberExpression, (union astNodeValue) {.number = 1}, createTypeNode(TYPE_BOOLEAN, 0, 0), flag_null, 0);
}
| FALSE {
    $$ = buildAstNode(numberExpression, (union astNodeValue) {.number = 0}, createTypeNode(TYPE_BOOLEAN, 0, 0), flag_null, 0);
}
| IDENTIFIER {
    $$ = buildAstNode(variableReferenceExpression, (union astNodeValue) {.string = $1}, 0, flag_null, 0);
}
| expression EQUALS expression {
    $$ = buildAstNode(assignExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
}
| expression PLUS expression {
    $$ = buildAstNode(addExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
}
| expression MINUS expression {
    $$ = buildAstNode(subtractExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
}
| expression MULTIPLY expression {
    $$ = buildAstNode(multiplyExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
}
| expression DIVIDE expression {
    $$ = buildAstNode(divideExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
}

type: IDENTIFIER {
    $$ = createTypeNode(TYPE_UNKNOWN, $1, 0);
}
| BOOLEAN {
    $$ = createTypeNode(TYPE_BOOLEAN, 0, 0);
}
| CHAR {
    $$ = createTypeNode(TYPE_CHAR, 0, 0);
}
| I8 {
    $$ = createTypeNode(TYPE_I8, 0, 0);
}
| I16 {
    $$ = createTypeNode(TYPE_I16, 0, 0);
}
| I32 {
    $$ = createTypeNode(TYPE_I32, 0, 0);
}
| I64 {
    $$ = createTypeNode(TYPE_I64, 0, 0);
}
| ISIZE {
    $$ = createTypeNode(TYPE_ISIZE, 0, 0);
}
| U8 {
    $$ = createTypeNode(TYPE_U8, 0, 0);
}
| U16 {
    $$ = createTypeNode(TYPE_U16, 0, 0);
}
| U32 {
    $$ = createTypeNode(TYPE_U32, 0, 0);
}
| U64 {
    $$ = createTypeNode(TYPE_U64, 0, 0);
}
| USIZE {
    $$ = createTypeNode(TYPE_USIZE, 0, 0);
}
| F32 {
    $$ = createTypeNode(TYPE_F32, 0, 0);
}
| F64 {
    $$ = createTypeNode(TYPE_F64, 0, 0);
}
| PTR LESS_THAN type GREATER_THAN {
    $$ = createTypeNode(TYPE_POINTER, 0, $3);
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
    error(fileName, yylloc.first_line, yylloc.first_column, message);
}

int parse(FILE* input, const char* file, struct astNode** ast) {
    const char* shouldDebug = getenv("AUL_PARSE_DEBUG");
    if (shouldDebug != 0 && strcmp(shouldDebug, "y") == 0) {
        yydebug = 1;
    }else {
        yydebug = 0;
    }
    yyin = input;
    fileName = file;
    int parseResult = yyparse();
    *ast = astRoot;
        return parseResult;
}
