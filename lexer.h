#ifndef AUL_LEXER_H
#define AUL_LEXER_H

#include <fstream>

#include "location.hh"

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL  aul::Parser::symbol_type aul::Lexer::next()

#include "aul.tab.hh"

namespace aul {
    class Lexer : public yyFlexLexer {
        private:
         location currentLocation;
        public:
        Lexer(std::istream& input) : yyFlexLexer(&input) {}
        ~Lexer() {}
         Parser::symbol_type next();
         location getLocation() { return currentLocation; }
        private:
         void updateLocation(const char* token);
    };
}

#endif