#ifndef AUL_ERROR_H
#define AUL_ERROR_H

#include <string>

namespace aul {
void error(std::string fileName, int line, int column, std::string message);
}

#endif