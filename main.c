#include "ast.h"
#include "aul.tab.h"
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
  if (result != 0) {
  }else {
    if (argc >= 2) {
      fclose(input);
    }
    dumpTree(ast);
  }
  if (result != 0) {
    fprintf(stderr, "Compilation aborted.");
  }
  return result;
}
