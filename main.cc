#include "aul.tab.hh"
#include "astPrinter.h"
#include "lexer.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>

using namespace aul;

using std::ifstream;
using std::istream;
using std::unique_ptr;

int main(int argc, char **argv) {
  istream *input;
  ifstream fileInput;
  const char *fileName;
  if (argc < 2) {
    input = &std::cin;
    fileName = "<stdin>";
  } else {
    fileInput = ifstream(argv[1]);
    input = &fileInput;
    fileName = argv[1];
    if (!fileInput) {
      perror("Error openning file");
      return 1;
    }
  }
  unique_ptr<AstNode> ast;
  Lexer lexer(*input);
  Parser parser(lexer, fileName, &ast);
  int result = parser();
  if (argc > 1) {
    fileInput.close();
  }
  if (result == 0) {
    AstPrinter astPrinter(std::cout);
    ast->apply(astPrinter);
  }
  if (result != 0) {
    fprintf(stderr, "Compilation aborted.\n");
  }
  return result;
}
