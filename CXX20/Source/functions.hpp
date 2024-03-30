#pragma once
#include <utility>

namespace details
{
    template<typename F, typename T, std::size_t... I>
    auto apply(F&& f, T&& t, std::index_sequence<I...>)
    {
        return std::invoke(
            std::forward<F>(f),
            std::get<I>(std::forward<T>(t))...
        );
    }
    
}


template<typename F, typename T>
auto apply(F&& f, T&& t)
{
    return details::apply(
        std::forward<F>(f),
        std::forward<T>(t),
        std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>{}
    );
}
