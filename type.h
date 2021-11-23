#ifndef AUL_TYPE_H
#define AUL_TYPE_H

enum typeType {
    TYPE_NONE,
  TYPE_CHAR,
  TYPE_I8,
  TYPE_I16,
  TYPE_I32,
  TYPE_I64,
  TYPE_ISIZE,
  TYPE_U8,
  TYPE_U16,
  TYPE_U32,
  TYPE_U64,
  TYPE_USIZE,
  TYPE_F32,
  TYPE_F64,
  TYPE_UNKNOWN, /*Packages etc.*/
  TYPE_POINTER
};

struct typeNode {
  enum typeType type;
  char* identifier; /*Mostly for TYPE_UNKNOWN*/
  struct typeNode* child; /*Mostly for TYPE_POINTER*/
};

struct typeNode* createTypeNode(enum typeType type, char* identifier, struct typeNode* child);

void printType(struct typeNode* node);

#endif