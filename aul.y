%require "3.8"
%define api.token.constructor
%define api.value.type variant
%define api.value.automove

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
#include "type.h"

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

%token AS "as"

%token BYTE "byte" %token INT16 "int16" %token INT32 "int32" %token INT64 "int64"
%token CHAR "char" %token BOOL "bool" %token FASTINT "fastint" %token SIZE "size"

%token SEMICOLON ";"

%token LEFT_PAREN "("
%token RIGHT_PAREN ")"
%token LEFT_BRACE "{"
%token RIGHT_BRACE "}"
%token LEFT_BRACKET "["
%token RIGHT_BRACKET "]"
%token DOT "."
%token COMMA ","
%token PIPE "|"
%token COLON ":"

%token INCREMENT "++"
%token DECREMENT "--"

%token PLUS "+"
%token MINUS "-"
%token STAR "*"
%token SLASH "/"
%token PERCENT "%"
%token CARET "^"
%token AMPERSAND "&"
%token LEFT_SHIFT "<<"
%token RIGHT_SHIFT ">>"

%token LESS "<"
%token GREATER ">"
%token EQUALS "="
%token EQUALS_EQUALS "=="
%token NOT_EQUALS "!="
%token LESS_EQUALS "<="
%token GREATER_EQUALS ">="

%token ARROW "->"

%token END 0 "EOF"

/*These tokens are not generated by the lexer, but they are necessary for use in context-dependent precedence*/
%token UNARY_MINUS "unary -"

%type <std::unique_ptr<aul::AstNode>> definition expression integer-expression cast-expression function-expression binary-expression unary-expression statement block-statement
%type <std::unique_ptr<aul::DefinitionsNode>> definitions
%type <std::unique_ptr<aul::Type>> type type-no-intersection array-type tuple-type function-type type-intersection
%type <std::vector<std::unique_ptr<Type>>> type-intersection-elements
%type <aul::PrimitiveTypeType> primitive-type
%type <std::vector<aul::NamedType>> tuple-type-elements
%type <aul::NamedType> tuple-type-element
%type <std::vector<std::unique_ptr<Type>>> type-list
%type <std::vector<std::unique_ptr<AstNode>>> statement-list

%right "="
%left "==" "!=" "<=" ">=" "<" ">"
%left "&" "|" "^"
%left "<<" ">>"
%left "+" "-"
%left "*" "/" "%"

%precedence "as"

%precedence "unary -"
%right "++" "--"

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
    auto definitions = $1;
    definitions->add($2);
    $$ = move(definitions);
}

definition: "const" IDENTIFIER "=" expression ";" {
    $$ = make_unique<DefinitionNode>(true, $2, $4);
}
| "let" IDENTIFIER "=" expression ";" {
    $$ = make_unique<DefinitionNode>(false, $2, $4);
}

expression: integer-expression | cast-expression | function-expression | binary-expression | unary-expression
| "(" expression ")" {
    $$ = $2;
    }

cast-expression: expression "as" type {
    $$ = make_unique<CastNode>($3, $1);
}

integer-expression: INTEGER {
    $$ = make_unique<IntegerNode>($1);
}

function-expression: "(" tuple-type-elements ")" "->" "{" statement-list "}" {
    $$ = make_unique<FunctionNode>($2, $6);
}

type: type-no-intersection | type-intersection{$$=$1;}

type-no-intersection:
primitive-type {$$ = make_unique<PrimitiveType>($1);}
| array-type
| tuple-type
| function-type

primitive-type:
  "byte" {$$ = PrimitiveTypeType::BYTE;}
| "int16" {$$ = PrimitiveTypeType::INT16;}
| "int32" {$$ = PrimitiveTypeType::INT32;}
| "int64" {$$ = PrimitiveTypeType::INT64;}
| "char" {$$ = PrimitiveTypeType::CHAR;}
| "bool" {$$ = PrimitiveTypeType::BOOL;}
| "fastint" {$$ = PrimitiveTypeType::FASTINT;}
| "size" {$$ = PrimitiveTypeType::SIZE;}

array-type: type "[" "]" {
    $$ = make_unique<ArrayType>($1);
}

tuple-type: "{" tuple-type-elements "}" {
    $$ = make_unique<TupleType>($2);
}

tuple-type-elements: 
%empty {
    $$ = std::vector<NamedType>();
}
| tuple-type-element {
    // Unfortunately, we can't just use an initializer list here. Don't ask me why.
    std::vector<NamedType> elements;
    elements.push_back($1);
    $$ = move(elements);
}
| tuple-type-elements "," tuple-type-element {
    auto elements = $1;
    elements.push_back($3);
    $$ = move(elements);
}

tuple-type-element: IDENTIFIER ":" type {
    $$ = {$1, $3};
}

type-intersection: "<" type-intersection-elements ">" {
    $$ = make_unique<TypeIntersection>($2);
}

type-intersection-elements:
 type-intersection-elements "|" type-no-intersection {
    auto types = $1;
    types.push_back($3);
    $$ = move(types);
}
| type-no-intersection "|" type-no-intersection {
    // Don't ask me why we can't use an initializer list here either.
    std::vector<std::unique_ptr<Type>> types;
    types.push_back($1);
    types.push_back($3);
    $$ = move(types);
}

function-type: type "(" type-list ")" {
    $$ = make_unique<FunctionType>($1, $3);
}

type-list:
type{
    // For some strange reason we can't use initializer lists.
    std::vector<std::unique_ptr<Type>> types;
    types.push_back($1);
    $$ = move(types);
}
| %empty {
    $$ = std::vector<std::unique_ptr<Type>>();
}
| type-list "," type {
    auto types = $1;
    types.push_back($3);
    $$ = move(types);
}

statement: block-statement | expression ";" | definition

block-statement: "{" statement-list "}" {
    $$ = make_unique<BlockStatementNode>($2);
}

statement-list:
%empty {
    $$ = std::vector<std::unique_ptr<AstNode>>();
}   
| statement-list statement {
    auto statements = $1;
    statements.push_back($2);
    $$ = move(statements);
}

binary-expression:
  expression "+" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::ADD, $1, $3);
  }
| expression "-" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::SUBTRACT, $1, $3);
  }
| expression "*" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::MULTIPLY, $1, $3);
  }
| expression "/" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::DIVIDE, $1, $3);
}
| expression "%" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::MODULO, $1, $3);
}
| expression "&" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::BITWISE_AND, $1, $3);
}
| expression "|" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::BITWISE_OR, $1, $3);
}
| expression "^" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::BITWISE_XOR, $1, $3);
}
| expression "<<" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::BITWISE_LEFT_SHIFT, $1, $3);
}
| expression ">>" expression {
    $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::BITWISE_RIGHT_SHIFT, $1, $3);
}
    | expression "=" expression {
        $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::ASSIGN, $1, $3);
    }
    | expression "==" expression {
        $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::EQUAL, $1, $3);
    }
    | expression "!=" expression {
        $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::NOT_EQUAL, $1, $3);
    }
    | expression "<" expression {
        $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::LESS, $1, $3);
    }
    | expression "<=" expression {
        $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::LESS_EQUAL, $1, $3);
    }
    | expression ">" expression {
        $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::GREATER, $1, $3);
    }
    | expression ">=" expression {
        $$ = make_unique<BinaryExpressionNode>(BinaryOperatorType::GREATER_EQUAL, $1, $3);
    }

unary-expression:
"-" expression %prec "unary -" {
    $$ = make_unique<UnaryExpressionNode>(UnaryOperatorType::NEGATE, $2);
}
| expression "++" {
    $$ = make_unique<UnaryExpressionNode>(UnaryOperatorType::POSTFIX_INCREMENT, $1);
}
| expression "--" {
    $$ = make_unique<UnaryExpressionNode>(UnaryOperatorType::POSTFIX_DECREMENT, $1);
}
| "++" expression {
    $$ = make_unique<UnaryExpressionNode>(UnaryOperatorType::PREFIX_INCREMENT, $2);
}
| "--" expression {
    $$ = make_unique<UnaryExpressionNode>(UnaryOperatorType::PREFIX_DECREMENT, $2);
}

%%

void aul::Parser::error(const aul::location& location, const std::string& message) {
    aul::error(fileName, location.begin.line, location.begin.column, message);
}
