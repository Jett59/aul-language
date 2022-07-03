%define api.token.constructor
%define api.value.type variant

%define api.namespace { aul }

%define api.parser.class  { Parser }

%code requires {

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
#include "error.h"

#include "lexer.h"

aul::Parser::symbol_type yylex(aul::Lexer& lexer) {
    return lexer.next();
}

%}

%lex-param { aul::Lexer& lexer }
%parse-param { aul::Lexer& lexer }
%parse-param { std::string fileName }

%token <std::string> IDENTIFIER "identifier"
%token <uintmax_t> INTEGER "integer"

%token LEFT_PAREN "("
%token RIGHT_PAREN ")"
%token LEFT_BRACE "{"
%token RIGHT_BRACE "}"
%token LEFT_BRACKET "["
%token RIGHT_BRACKET "]"

%token LESS "<"
%token GREATER ">"
%token EQUALS "="
%token EQUALS_EQUALS "=="
%token LESS_EQUALS "<="
%token GREATER_EQUALS ">="

%token END 0 "EOF"

%start definitions

%%

definitions: IDENTIFIER;

%%

void aul::Parser::error(const aul::location& location, const std::string& message) {
    aul::error(fileName, location.begin.line, location.begin.column, message);
}
