%option nodefault
%option noyywrap

%{

#include <cstdint>
#include "aul.tab.hh"
#include "location.hh"
#include "lexer.h"

#define YY_USER_ACTION updateLocation(yytext);

#define MAKE(TYPE)  Parser::make_ ## TYPE (currentLocation)
#define MAKE_PARAMS(TYPE, ...) Parser::make_ ## TYPE (__VA_ARGS__, currentLocation)

#define yyterminate() MAKE(END)

static uintmax_t toInt(const char *str) {
    uintmax_t result = 0;
    while (*str) {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

%}

%option c++
%option yyclass="Lexer"

%%

"//".* ;

"\n"|"\r"|" "|"\t" ;

"let" return MAKE(LET);
"const" return MAKE(CONST);

"as" return MAKE(AS);

"byte" return MAKE(BYTE);
"int16" return MAKE(INT16);
"int32" return MAKE(INT32);
"int64" return MAKE(INT64);
"char" return MAKE(CHAR);
"bool" return MAKE(BOOL);
"fastint" return MAKE(FASTINT);
"size" return MAKE(SIZE);

[a-zA-Z_][a-zA-Z0-9_]* return MAKE_PARAMS(IDENTIFIER, yytext);
[0-9]+ return MAKE_PARAMS(INTEGER, toInt(yytext));

";" return MAKE(SEMICOLON);

"(" return MAKE(LEFT_PAREN);
")" return MAKE(RIGHT_PAREN);
"{" return MAKE(LEFT_BRACE);
"}" return MAKE(RIGHT_BRACE);
"[" return MAKE(LEFT_BRACKET);
"]" return MAKE(RIGHT_BRACKET);
"." return MAKE(DOT);
"," return MAKE(COMMA);
"|" return MAKE(PIPE);
":" return MAKE(COLON);

"++" return MAKE(INCREMENT);
"--" return MAKE(DECREMENT);

"+" return MAKE(PLUS);
"-" return MAKE(MINUS);
"*" return MAKE(STAR);
"/" return MAKE(SLASH);
"%" return MAKE(PERCENT);
"^" return MAKE(CARET);
"&" return MAKE(AMPERSAND);
"<<" return MAKE(LEFT_SHIFT);
">>" return MAKE(RIGHT_SHIFT);

"<" return MAKE(LESS);
">" return MAKE(GREATER);
"=" return MAKE(EQUALS);
"==" return MAKE(EQUALS_EQUALS);
"<=" return MAKE(LESS_EQUALS);
">=" return MAKE(GREATER_EQUALS);

"->" return MAKE(ARROW);

<<EOF>> return yyterminate();

. {
    std::cerr << "Unknown character: " << yytext << std::endl;
    return yyterminate();
}

%%

void aul::Lexer::updateLocation(const char* token) {
    currentLocation.begin = currentLocation.end;
    int tokenLength = strlen(token);
    for (int i = 0; i < tokenLength; i ++) {
        char c = token[i];
        if (c == '\n') {
            currentLocation.end.line++;
            currentLocation.end.column = 1;
        }else {
            currentLocation.end.column++;
        }
    }
}
