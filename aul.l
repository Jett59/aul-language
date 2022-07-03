%option nodefault
%option noyywrap

%{

#include "aul.tab.hh"
#include "location.hh"
#include "error.h"
#include "lexer.h"

extern fileName;

#define YY_USER_ACTION updateLocation(yytext);

#define yyterminate()  Parser::make_END(currentLocation);

#define MAKE(TYPE)  Parser::make_ ## TYPE (currentLocation)
#define MAKE_PARAMS(TYPE, ...) Parser::make_ ## TYPE (currentLocation, __VA_ARGS__)

%}

%option c++
%option yyclass="Lexer"

%%

"//".*   ;

"\n"|" "|"\t"   ;

.  return yyterminate();

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
