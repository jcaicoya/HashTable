#pragma once

#include <functional>
#include <string_view>
#include <map>


class HashFunction
{
public:
    enum class Type { Identity, Square, Cube };
    using HashFoo = std::function<size_t (int)>;

    HashFunction(Type type, HashFoo foo)
        : _type(type)
        , _foo(foo)
    {}

    std::string_view to_string() const;
    std::size_t operator()(int x) const { return _foo(x); }

private:
    const Type _type;
    const HashFoo _foo;
};


class HashFunctionFactory
{
public:
    static HashFunction Build(HashFunction::Type type);
};

