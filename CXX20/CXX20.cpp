#include <algorithm>
#include <string>
#include <chrono>
#include <variant>
#include <iostream>
#include <random>
#include <span>
#include "Source/string_helpers.hpp"
#include <type_traits>
using namespace std::string_literals;

using long_type = std::conditional_t<
        sizeof(void*) <= 4, long, long long
    >;

template<int size>
using number_type =
    typename std::conditional_t<
        size<=1,
        std::int8_t,
        typename std::conditional_t<
            size<=2,
            std::int16_t,
            typename std::conditional_t<
                size<=4,
                std::int32_t,
                std::int64_t
                >
            >
        >;

template<typename T,
         typename D = std::conditional_t<
            std::is_integral_v<T>,
            std::uniform_int_distribution<T>,
            std::uniform_real_distribution<T>
             >,
        typename = std::enable_if_t<std::is_arithmetic_v<T>>
>
std::vector<T> generate_random(T const min, T const max, size_t const size)
{
    std::vector<T> v(size);

    std::random_device rd;
    std::mt19937 mt(rd());

    D dist(min, max);

    std::generate(std::begin(v), std::end(v),
        [&dist, &mt] {return dist(mt);});

    return v;
}

int main(int argc, char* argv[])
{
    auto v1 = generate_random(1, 10, 10);
    auto v2 = generate_random(0.0, 1.0, 10);

    for (auto value : v1)
    {
        std::cout << value << ' ';
    }

    std::cout << '\n';

    for (auto value : v2)
    {
        std::cout << value << ' ';
    }
}

