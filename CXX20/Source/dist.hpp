#pragma once
#include <random>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>

inline void generate_and_print(std::function<int(void)> gen, int const iterations = 10000)
{
    // map to store the numbers and their repetitions
    auto map = std::map<int, int>{};
    
    // generate random numbers
    for(auto n = 0; n < iterations; ++n)
    {
        ++map[gen()];
    }

    // find the element with the most repetition
    auto max = std::max_element(
        std::begin(map), std::end(map), [](auto kvp1, auto kvp2)
        {
            return kvp1.second < kvp2.second;
        });

    // print the bars
    for(auto i = max->second / 200; i > 0; --i)
    {
        for(auto const& kvp : map)
        {
            std::cout <<
                std::fixed << std::setprecision(1) << std::setw(3)
            << (kvp.second / 200 >= i ? char(219) : ' ');
        }
    }
    
    std::cout << '\n';

    // print the numbers
    for(auto const& kvp : map)
    {
        std::cout <<
                std::fixed << std::setprecision(1) << std::setw(3)
            << kvp.first;
    }

    std::cout << '\n';
}