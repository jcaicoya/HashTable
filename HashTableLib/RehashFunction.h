#pragma once


#include <functional>
#include <string_view>
#include <map>


class RehashFunction
{
public:
    enum class Type { Next, Square, Cube };
    using RehashFoo = std::function<std::size_t (std::size_t)>;

    RehashFunction(Type type, RehashFoo foo)
        : _type(type)
        , _foo(foo)
    {}

    Type getType() const { return _type; }
    RehashFoo getFoo() const { return _foo; }
    std::string_view to_string() const;
    std::size_t operator()(std::size_t iteration) const { return _foo(iteration); }

    static std::string_view to_string(const RehashFunction &rehasFunction);
    static std::string_view to_string(Type type);

private:
    const Type _type;
    const RehashFoo _foo;
};


class RehashFunctionFactory
{
public:
    static RehashFunction Build(RehashFunction::Type type);
};
