#include <stdio.h>
#include "error.h"

namespace aul {
void error(const char* fileName, int line, int column, const char* message) {
  fprintf(stderr, "%s:%d:%d:\n%s\n", fileName, line, column, message);
}
}
