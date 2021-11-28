#ifndef AUL_ERROR_H
#define AUL_ERROR_H

namespace aul {
void error(const char* fileName, int line, int column, const char* message);
}

#endif