#ifndef AUL_LEXER_H
#define AUL_LEXER_H

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL  aul::Parser::symbol_type aul::Lexer::next()

#include "aul.tab.hh"

namespace aul {
    class Lexer : public yyFlexLexer {
        public:
        Lexer() {}
        ~Lexer() {}
         Parser::symbol_type next();
    };
}

#endif