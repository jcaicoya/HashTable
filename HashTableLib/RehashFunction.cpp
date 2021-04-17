#include "RehashFunction.h"

std::string_view RehashFunction::to_string() const
{
    return to_string(_type);
}


std::string_view RehashFunction::to_string(const RehashFunction &rehasFunction)
{
    return to_string(rehasFunction._type);
}


std::string_view RehashFunction::to_string(Type type)
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
        default:
            return Identity;
    }
}


RehashFunction RehashFunctionFactory::Build(RehashFunction::Type type)
{
    switch (type)
    {
        case RehashFunction::Type::Identity:
            return RehashFunction(RehashFunction::Type::Identity, RehashFunction::RehashFoo([](size_t iteration) { return iteration; }));
        case RehashFunction::Type::Square:
             return RehashFunction(RehashFunction::Type::Square, RehashFunction::RehashFoo([](size_t iteration) { return iteration * iteration; }));
        case RehashFunction::Type::Cube:
             return RehashFunction(RehashFunction::Type::Cube, RehashFunction::RehashFoo([](size_t iteration) { return iteration * iteration * iteration; }));
        default:
            return RehashFunction(RehashFunction::Type::Identity, RehashFunction::RehashFoo([](size_t iteration) { return iteration; }));
    }
}
