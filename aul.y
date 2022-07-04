%define api.token.constructor
%define api.value.type variant

%define api.namespace { aul }

%define api.parser.class  { Parser }

%code requires {
    #include "ast.h"

    namespace aul {
        class Lexer;
    }
}

%debug

%locations

%define parse.error verbose

%{

#include <cstdint>
#include <iostream>
#include <memory>
#include "error.h"
#include "ast.h"
#include "lexer.h"

aul::Parser::symbol_type yylex(aul::Lexer& lexer) {
    return lexer.next();
}

using std::make_unique;
using std::move;

%}

%lex-param { aul::Lexer& lexer }
%parse-param { aul::Lexer& lexer }
%parse-param { std::string fileName }
%parse-param {std::unique_ptr<aul::AstNode> *ast}

%token <std::string> IDENTIFIER "identifier"
%token <uintmax_t> INTEGER "integer"

%token LET "let"
%token CONST "const"

%token SEMICOLON ";"

%token LEFT_PAREN "("
%token RIGHT_PAREN ")"
%token LEFT_BRACE "{"
%token RIGHT_BRACE "}"
%token LEFT_BRACKET "["
%token RIGHT_BRACKET "]"
%token DOT "."

%token LESS "<"
%token GREATER ">"
%token EQUALS "="
%token EQUALS_EQUALS "=="
%token LESS_EQUALS "<="
%token GREATER_EQUALS ">="

%token END 0 "EOF"

%type <std::unique_ptr<aul::DefinitionsNode>> definitions
%type <std::unique_ptr<aul::AstNode>> definition expression integer-expression

%start compilation_unit

%%

compilation_unit: definitions {
    *ast = move($1);
}

definitions: 
%empty {
    $$ = make_unique<DefinitionsNode>();
}
| definitions definition {
    $1->add(move($2));
    $$ = move($1);
}

definition: "const" IDENTIFIER "=" expression ";" {
    $$ = make_unique<DefinitionNode>(true, move($2), move($4));
}
| "let" IDENTIFIER "=" expression ";" {
    $$ = make_unique<DefinitionNode>(false, move($2), move($4));
}

expression: integer-expression {$$ = move($1);}

integer-expression: INTEGER {
    $$ = make_unique<IntegerNode>($1);
}

%%

void aul::Parser::error(const aul::location& location, const std::string& message) {
    aul::error(fileName, location.begin.line, location.begin.column, message);
}
