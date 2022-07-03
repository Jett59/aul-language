#include "lexer.h"
#include "aul.tab.hh"
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
  Parser parser(lexer, fileName);
  int result;
  result = parser();
  if (argc > 1) {
    fileInput.close();
  }
    if (result != 0) {
    fprintf(stderr, "Compilation aborted.\n");
  }
  return result;
}
