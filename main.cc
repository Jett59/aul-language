#include "ast.h"
#include "lexer.h"
#include "aul.tab.hh"
#include "semantics.h"
#include "symbols.h"
#include <stdio.h>

using namespace aul;

int main(int argc, char **argv) {
  FILE *input;
  const char *fileName;
  if (argc < 2) {
    input = stdin;
    fileName = "<stdin>";
  } else {
    input = fopen(argv[1], "r");
    fileName = argv[1];
    if (input == 0) {
      perror("Error openning file");
      return 1;
    }
  }
  Lexer lexer;
  Parser parser(lexer);
  int result;
  astNode *ast;
  result = parser();
  if (argc >= 2) {
    fclose(input);
  }
  if (result == 0) {
    result = buildSymbolTable(&ast, 0);
    if (result == 0) {
      result = analyseSemantics(ast);
      if (result == 0) {
        printTree(ast);
      }
    }
  }
  if (result != 0) {
    fprintf(stderr, "Compilation aborted.\n");
  }
  return result;
}
