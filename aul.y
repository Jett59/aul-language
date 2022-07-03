%define api.token.constructor
%define api.value.type variant

%define api.namespace { aul }

%define api.parser.class  { Parser }

%code requires {

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

aul::Parser::symbol_type yylex(aul::Lexer& lexer) {
    return lexer.next();
}

%}

%lex-param { aul::Lexer& lexer }
%parse-param { aul::Lexer& lexer }
%parse-param { const char* fileName }
%parse-param { astNode** ast }

%%

%%

void aul::Parser::error(aul::location const& location, const std::string& message) {
    aul::error(fileName, location.begin.line, location.begin.column, message.c_str());
}
