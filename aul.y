%define api.token.constructor
%define api.value.type variant

%define api.namespace { aul }

%define api.parser.class  { Parser }

%code requires {
    #include "ast.h"
    #include "type.h"
    #include <stdio.h>

    namespace aul {
        class Lexer;
    }
}

%debug

%locations

%define parse.error verbose

%{

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#include "lexer.h"

void yyerror(const char* message);

#define buildAstNode(location, nodeType, value, type, numChildren, ...)  createAstNode(fileName, location.begin.line, location.begin.column, nodeType, value, type, numChildren, __VA_ARGS__);

aul::Parser::symbol_type yylex(aul::Lexer& lexer) {
    return lexer.next();
}

%}

%lex-param { aul::Lexer& lexer }
%parse-param { aul::Lexer& lexer }
%parse-param { const char* fileName }
%parse-param { astNode** ast }

%token <char*> IDENTIFIER 
%token <double> INTEGER DECIMAL
%token LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE EQUALS COMMA DOT SEMICOLON
%token LESS_THAN GREATER_THAN PLUS MINUS MULTIPLY DIVIDE
%token MODULE PACKAGE
%token EXPORT INTERNAL
%token INSTANCE STATIC
%token RETURN
%token BOOLEAN CHAR I8 I16 I32 I64 ISIZE U8 U16 U32 U64 USIZE F32 F64 PTR
%token TRUE FALSE

%token END  0

%nterm <char*> dottedIdentifier
%nterm <typeNode*> type
%type <astNode*> program moduleDeclaration packageDefinition definitions definition statement statements localVariableDefinition variableDefinition functionDefinition argumentList argument returnStatement expression
%type <astNodeFlags> visibility scope

%right EQUALS
%left PLUS MINUS
%left MULTIPLY DIVIDE

%start program

%%

program: moduleDeclaration packageDefinition {
    $$ = buildAstNode(@$, program, (union astNodeValue) {}, 0, flag_null, 2, $1, $2);
    *ast = $$;
}

moduleDeclaration: MODULE dottedIdentifier SEMICOLON {
    $$ = buildAstNode(@$, moduleDeclaration, (union astNodeValue) {.string = $2}, 0, flag_null, 0);
}

packageDefinition: visibility PACKAGE IDENTIFIER LEFT_BRACE definitions RIGHT_BRACE {
    $$ = buildAstNode(@$, packageDefinition, (union astNodeValue) {.string = $3}, 0, $1, 1, $5);
}

definitions: definitions definition {
    $$ = addAstNode(&$1, $2);
}
| {
    $$ = buildAstNode(@$, definitions, (union astNodeValue) {}, 0, flag_null, 0);
}

definition: variableDefinition | functionDefinition;

statements: statements statement {
    $$ = addAstNode(&$1, $2);
}
| {
    $$ = buildAstNode(@$, statements, (union astNodeValue) {}, 0, flag_null, 0);
}

statement: returnStatement | expression SEMICOLON | localVariableDefinition;

localVariableDefinition: type IDENTIFIER EQUALS expression SEMICOLON {
    $$ = buildAstNode(@$, variableDefinition, (union astNodeValue) {.string = $2}, $1, flag_null, 1, $4);
}

variableDefinition: visibility scope type IDENTIFIER EQUALS expression SEMICOLON {
    $$ = buildAstNode(@$, variableDefinition, (union astNodeValue) {.string = $4}, $3, $1 | $2, 1, $6);
}

functionDefinition: visibility scope type IDENTIFIER LEFT_PAREN argumentList RIGHT_PAREN LEFT_BRACE statements RIGHT_BRACE {
    $$ = buildAstNode(@$, functionDefinition, (union astNodeValue) {.string = $4}, $3, $1 | $2, 2, $6, $9);
}
| visibility scope IDENTIFIER LEFT_PAREN argumentList RIGHT_PAREN LEFT_BRACE statements RIGHT_BRACE {
    $$ = buildAstNode(@$, functionDefinition, (union astNodeValue) {.string = $3}, createTypeNode(TYPE_NONE, 0, 0), $1 | $2, 2, $5, $8);
}

argumentList: argumentList COMMA argument {
    $$ = addAstNode(&$1, $3);
}
| argument {
    $$ = buildAstNode(@$, argumentList, (union astNodeValue) {}, 0, flag_null, 1, $1);
}
| {
    $$ = buildAstNode(@$, argumentList, (union astNodeValue) {}, 0, flag_null, 0);
}

argument: type IDENTIFIER {
    $$ = buildAstNode(@$, variableDefinition, (union astNodeValue) {.string = $2}, $1, flag_null, 0);
}

returnStatement: RETURN expression SEMICOLON {
    $$ = buildAstNode(@$, returnStatement, (union astNodeValue) {}, 0, flag_null, 1, $2);
}

expression: LEFT_PAREN expression RIGHT_PAREN {
    $$ = $2;
}
| DECIMAL {
    $$ = buildAstNode(@$, numberExpression, (union astNodeValue) {.number = $1}, createTypeNode(TYPE_F64, 0, 0), flag_null, 0);
}
| INTEGER {
    $$ = buildAstNode(@$, numberExpression, (union astNodeValue) {.number = $1}, createTypeNode(TYPE_I64, 0, 0), flag_null, 0);
}
| TRUE {
    $$ = buildAstNode(@$, numberExpression, (union astNodeValue) {.number = 1}, createTypeNode(TYPE_BOOLEAN, 0, 0), flag_null, 0);
}
| FALSE {
    $$ = buildAstNode(@$, numberExpression, (union astNodeValue) {.number = 0}, createTypeNode(TYPE_BOOLEAN, 0, 0), flag_null, 0);
}
| IDENTIFIER {
    $$ = buildAstNode(@$, variableReferenceExpression, (union astNodeValue) {.string = $1}, 0, flag_null, 0);
}
| type LEFT_PAREN expression RIGHT_PAREN {
    $$ = buildAstNode(@$, castExpression, (union astNodeValue) {}, $1, flag_null, 1, $3);
}
| expression EQUALS expression {
    $$ = buildAstNode(@$, assignExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
}
| expression PLUS expression {
    $$ = buildAstNode(@$, addExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
}
| expression MINUS expression {
    $$ = buildAstNode(@$, subtractExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
}
| expression MULTIPLY expression {
    $$ = buildAstNode(@$, multiplyExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
}
| expression DIVIDE expression {
    $$ = buildAstNode(@$, divideExpression, (union astNodeValue) {}, 0, flag_null, 2, $1, $3);
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
    char* newStr = (char*)malloc(strlen($1) + strlen($3) + 2);
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

void aul::Parser::error(aul::location const& location, const std::string& message) {
    std::cerr << message << std::endl;
}
