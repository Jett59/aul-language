#include "ast.h"
#include "aul.tab.h"
#include "symbols.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *input;
  if (argc < 2) {
    input = stdin;
  } else {
    input = fopen(argv[1], "r");
  }
  int result;
  struct astNode *ast;
  result = parse(input, &ast);
  if (argc >= 2) {
    fclose(input);
  }
  if (result == 0) {
    result = buildSymbolTable(&ast, 0);
    if (result == 0) {
      printTree(ast);
    }
  }
  if (result != 0) {
    fprintf(stderr, "Compilation aborted.");
  }
  return result;
}
