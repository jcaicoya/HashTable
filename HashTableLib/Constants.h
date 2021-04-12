#pragma once

#include <functional>
#include <string_view>
#include <map>




namespace Constants
{

inline std::function<size_t (int)> identity([](int x) { return x; });
inline std::function<size_t (int, size_t)> next([](int /*value*/, size_t current) { return current + 1;});

} // end namespace Constants
