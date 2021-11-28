#include "ast.h"
#include "lexer.h"
#include "aul.tab.hh"
#include "semantics.h"
#include "symbols.h"
#include <cstdio>
#include <fstream>
#include <iostream>

using namespace aul;

using std::istream;

int main(int argc, char **argv) {
  istream *input;
  std::ifstream fileInput;
  const char *fileName;
  if (argc < 2) {
    input = &std::cin;
    fileName = "<stdin>";
  } else {
    fileInput = std::ifstream(argv[1]);
    input = &fileInput;
    fileName = argv[1];
    if (!fileInput) {
      perror("Error openning file");
      return 1;
    }
  }
  Lexer lexer(*input);
  astNode *ast;
  Parser parser(lexer, fileName, &ast);
  int result;
  result = parser();
  if (argc > 1) {
    fileInput.close();
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
