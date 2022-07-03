#include <iostream>
#include "error.h"

using std::cerr;
using std::endl;

namespace aul {
void error(std::string fileName, int line, int column, std::string message) {
  cerr << fileName << ":" << line << ":" << column << ":" << endl;
  cerr << message << endl;
}
}
