%option nodefault
%option noyywrap

%{

#include <stdlib.h>
#include "aul.tab.hh"
#include "location.hh"
#include "error.h"
#include "lexer.h"

extern const char* fileName;

void yyerror(const char* message);

void storeString(void);
void storeNumber(void);

#define yyterminate()  Parser::make_END(location());

#define MAKE(TYPE)  Parser::make_ ## TYPE (location())

%}

%option c++
%option yyclass="Lexer"

%%

"//".*   ;

"\n"|" "|"\t"   ;

"export"  return MAKE(EXPORT);
"internal"  return MAKE(INTERNAL);
"static"  return MAKE(STATIC);
"instance"  return MAKE(INSTANCE);
"module"  return MAKE(MODULE);
"package"  return MAKE(PACKAGE);
"return"  return MAKE(RETURN);

"true"  return MAKE(TRUE);
"false"  return MAKE(FALSE);

"boolean"  return MAKE(BOOLEAN);
"char"  return MAKE(CHAR);
"i8"  return MAKE(I8);
"i16"  return MAKE(I16);
"i32"  return MAKE(I32);
"i64"  return MAKE(I64);
"isize"  return MAKE(ISIZE);
"u8"  return MAKE(U8);
"u16"  return MAKE(U16);
"u32"  return MAKE(U32);
"u64"  return MAKE(U64);
"usize"  return MAKE(USIZE);
"f32"  return MAKE(F32);
"f64"  return MAKE(F64);
"ptr"  return MAKE(PTR);

[a-zA-Z\_][a-zA-Z\_0-9]*   return Parser::make_IDENTIFIER(yytext, location());
[0-9]+"."[0-9]+   return Parser::make_DECIMAL(atof(yytext), location());
[0-9]+  return Parser::make_INTEGER(atof(yytext), location());

"("  return MAKE(LEFT_PAREN);
")"  return MAKE(RIGHT_PAREN);
"["  return MAKE(LEFT_BRACKET);
"]"  return MAKE(RIGHT_BRACKET);
"{"  return MAKE(LEFT_BRACE);
"}"  return MAKE(RIGHT_BRACE);
"="  return MAKE(EQUALS);
","  return MAKE(COMMA);
"."  return MAKE(DOT);
";"  return MAKE(SEMICOLON);

"<"  return MAKE(LESS_THAN);
">"  return MAKE(GREATER_THAN);

"+"  return MAKE(PLUS);
"-"  return MAKE(MINUS);
"*"  return MAKE(MULTIPLY);
"/"  return MAKE(DIVIDE);

.  return yyterminate()

%%