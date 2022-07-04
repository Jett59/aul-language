#ifndef AUL_TYPE_H
#define AUL_TYPE_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace aul {
enum class TypeCategory { PRIMITIVE, ARRAY, TUPLE, INTERSECTION };
class Type {
public:
  virtual ~Type() {}

  virtual TypeCategory getTypeCategory()const = 0;

  virtual std::string toString()const  = 0;
};
enum class PrimitiveTypeType {
  BYTE,
  INT16,
  INT32,
  INT64,
  BOOL,
  CHAR,
  FASTINT,
  SIZE
};
class PrimitiveType : public Type {
public:
  PrimitiveType(PrimitiveTypeType type) : type(type) {}

  virtual TypeCategory getTypeCategory() const { return TypeCategory::PRIMITIVE; }

  PrimitiveTypeType getType() { return type; }

  virtual std::string toString() const {
    switch (type) {
    case PrimitiveTypeType::BYTE:
      return "byte";
    case PrimitiveTypeType::INT16:
      return "int16";
    case PrimitiveTypeType::INT32:
      return "int32";
    case PrimitiveTypeType::INT64:
      return "int64";
    case PrimitiveTypeType::BOOL:
      return "bool";
    case PrimitiveTypeType::CHAR:
      return "char";
    case PrimitiveTypeType::FASTINT:
      return "fastint";
    case PrimitiveTypeType::SIZE:
      return "size";
    default:
      return "Unknown";
    }
  }

private:
  PrimitiveTypeType type;
};
class Array : public Type {
public:
  Array(std::unique_ptr<Type> type) : type(std::move(type)) {}

  virtual TypeCategory getTypeCategory() const { return TypeCategory::ARRAY; }

  const Type &getType() { return *type; }

virtual std::string toString() const {
  return "array<" + type->toString() + ">";
}

private:
  std::unique_ptr<Type> type;
};
struct NamedType {
  std::string name;
  std::unique_ptr<Type> type;
};
class Tuple : public Type {
public:
  Tuple(std::vector<NamedType> types) : types(std::move(types)) {}

  void add(NamedType type) { types.push_back(std::move(type)); }

  virtual TypeCategory getTypeCategory() const { return TypeCategory::TUPLE; }

  const std::vector<NamedType> &getTypes() { return types; }

virtual std::string toString() const {
  std::string result = "tuple<";
  for (auto &type : types) {
    result += type.name + ":" + type.type->toString() + ",";
  }
  result.pop_back();
  result += ">";
  return result;
}

private:
  std::vector<NamedType> types;
};
class TypeIntersection : public Type {
public:
  TypeIntersection(std::vector<std::unique_ptr<Type>> types)
      : types(std::move(types)) {}

  void add(std::unique_ptr<Type> type) { types.push_back(std::move(type)); }

  virtual TypeCategory getTypeCategory() const { return TypeCategory::INTERSECTION; }

  const std::vector<std::unique_ptr<Type>> &getTypes() { return types; }

virtual std::string toString() const {
  std::string result = "intersection<";
  for (auto &type : types) {
    result += type->toString() + "|";
  }
  result.pop_back();
  result += ">";
  return result;
}

private:
  std::vector<std::unique_ptr<Type>> types;
};
} // namespace aul

#endif