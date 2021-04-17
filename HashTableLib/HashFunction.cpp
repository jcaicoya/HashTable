#include "HashFunction.h"

std::string_view HashFunction::to_string() const
{
   return to_string(_type);
}


std::string_view HashFunction::to_string(const HashFunction &hashFunction)
{
    return to_string(hashFunction._type);
}


std::string_view HashFunction::to_string(Type type)
{
    static std::string_view Identity = "Identity";
    static std::string_view Square = "Square";
    static std::string_view Cube = "Cube";

    switch (type)
    {
        case Type::Identity:
            return Identity;
        case Type::Square:
            return Square;
        case Type::Cube:
            return Cube;
    }
}


HashFunction HashFunctionFactory::Build(HashFunction::Type type)
{
    switch (type)
    {
        case HashFunction::Type::Identity:
            return HashFunction(HashFunction::Type::Identity, HashFunction::HashFoo([](int x) { return x; }));
        case HashFunction::Type::Square:
             return HashFunction(HashFunction::Type::Square, HashFunction::HashFoo([](int x) { return x * x; }));
        case HashFunction::Type::Cube:
             return HashFunction(HashFunction::Type::Cube, HashFunction::HashFoo([](int x) { return x * x * x; }));
    }
}


/*
const std::map<HashFunction::Type, HashFunction> HashFunctionFactory::_hashFoos
{
    { std::make_pair(HashFunction::Type::Identity, std::function<int (int)>([](int x) { return x; })) },
    { std::make_pair(HashFunction::Type::Square, std::function<int (int)>([](int x) { return x * x; })) },
    { std::make_pair(HashFunction::Type::Cube, std::function<int (int)>([](int x) { return x * x * x; })) }
};
*/


