#include <algorithm>
#include <string>
#include <chrono>
#include <variant>
#include <iostream>
#include <span>
#include "Source/string_helpers.hpp"
#include <type_traits>
using namespace std::string_literals;

struct foo
{
    std::string serialize()
    {
        return "plain"s;
    }
};

struct bar
{
    std::string serialize_with_encoding()
    {
        return "encoded"s;
    }
};


template<typename T>
struct is_serializable_with_encoding
{
    static constexpr bool value = false;
};

template<>
struct is_serializable_with_encoding<bar>
{
    static constexpr bool value = true;
};


template<bool b>
struct serializer
{
    template<typename T>
    static auto serialize(T& v)
    {
        return v.serialize();
    }
};

template<>
struct serializer<true>
{
    template<typename T>
    static auto serialize(T& v)
    {
        return v.serialize_with_encoding();
    }
};

template<typename T>
auto serialize(T& v)
{
    return serializer<is_serializable_with_encoding<T>::value>::serialize(v);
}



int main(int argc, char* argv[])
{
    foo f;
    bar b;
    std::cout << serialize(f) << std::endl;
    std::cout << serialize(b) << std::endl;
}

