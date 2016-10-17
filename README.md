# CTTI

[![Join the chat at https://gitter.im/Manu343726/ctti](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Manu343726/ctti?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Compile Time Type Information for the C++ programming language.

## Background

We know C++ is a statically typed compiled language, but it's disappointing that we cannot even get
the name of a C++ type at compile time, having to use RTTI (Run Time Type Information) which introduces
a lot of overhead. In fact, that's one of the most feared features of C++, usually disabled in performance dependent
scenarios such as videogames programming.

This library aims to provide features similar to RTTI `std::type_info` at compile-time, currently `constexpr` type name and
a `constexpr` replacement of `std::type_index` for indexing maps with types.

``` cpp
#include "ctti/type_id.hpp"

int main()
{
    static_assert(ctti::type_id<int>() != ctti::type_id("hello"), "compile-time type-id comparison");

    std::unordered_map<ctti::type_id_t, std::size_t> sizeof_map;

    sizeof_map[ctti::type_id<int>()] = sizeof(int);
}
```

## Support and current status

This was tested on Visual Studio 2015, GCC 5.2.0, MinGW GCC 5.1, Clang 3.6.2, and cygwin Clang 3.5.2.

## Acknowledgments

Thanks a lot to Jonathan "foonathan" Müller, both for the `constexpr` hash and the idea for the `__PRETTY_FUNCTION__` trick.

## License

This project is licensed under the MIT license. See LICENSE.md for more details.
