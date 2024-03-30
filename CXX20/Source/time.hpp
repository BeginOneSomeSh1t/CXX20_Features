#pragma once
#include <chrono>

template<typename Time = std::chrono::microseconds,
         typename Clock = std::chrono::high_resolution_clock>
struct performance_timer
{
    template<typename F, typename... Ts>
    static Time duration(F&& f, Ts... rest)
    {
        auto start = Clock::now();
        std::invoke(std::forward<F>(f), std::forward<Ts>(rest)...);
        auto end = Clock::now();

        return std::chrono::duration_cast<Time>(end - start);
    }
};