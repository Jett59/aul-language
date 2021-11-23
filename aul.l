%option noyywrap

%{

#include <stdlib.h>
#include "aul.tab.h"

void yyerror(const char* message);

void storeString(void);
void storeNumber(void);

%}

%%

"//".*   ;

"\n"|" "|"\t"   ;

"export"  return EXPORT;
"internal"  return INTERNAL;
"static"  return STATIC;
"instance"  return INSTANCE;
"module"  return MODULE;
"package"  return PACKAGE;
"return"  return RETURN;

"true"  return TRUE;
"false"  return FALSE;

"boolean"  return BOOLEAN;
"char"  return CHAR;
"i8"  return I8;
"i16"  return I16;
"i32"  return I32;
"i64"  return I64;
"isize"  return ISIZE;
"u8"  return U8;
"u16"  return U16;
"u32"  return U32;
"u64"  return U64;
"usize"  return USIZE;
"f32"  return F32;
"f64"  return F64;
"ptr"  return PTR;

[a-zA-Z\_][a-zA-Z\_0-9]*   storeString(); return IDENTIFIER;
[0-9]+"."[0-9]*   storeNumber(); return DECIMAL;
[0-9]+   storeNumber(); return INTEGER;

"("  return LEFT_PAREN;
")"  return RIGHT_PAREN;
"["  return LEFT_BRACKET;
"]"  return RIGHT_BRACKET;
"{"  return LEFT_BRACE;
"}"  return RIGHT_BRACE;
"="  return EQUALS;
","  return COMMA;
"."  return DOT;
";"  return SEMICOLON;

"<"  return LESS_THAN;
">"  return GREATER_THAN;

"+"  return PLUS;
"-"  return MINUS;

.   yyerror("Unknown character"); return 0;

%%

void storeString(void) {
    char* newString = malloc(strlen(yytext) + 1);
    strcpy(newString, yytext);
    yylval.string = newString;
}

void storeNumber(void) {
    yylval.number = atof(yytext);
}
