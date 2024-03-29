
#include <iostream>

template<typename T>
struct wrapper
{
    T const& value;
};

template<typename T>
constexpr auto operator<(wrapper<T> const& lhs, wrapper<T> const& rhs)
{
    return wrapper<T>{
        lhs.value < rhs.value ? lhs.value : rhs.value
    };
}

template<typename... Ts>
constexpr auto min(Ts&&... rest)
{
    return (wrapper<Ts>{rest} < ...).value;
}

int main(int argc, char* argv[])
{

    std::cout << min(23, 32, 3, 2, 43, 1);
}

