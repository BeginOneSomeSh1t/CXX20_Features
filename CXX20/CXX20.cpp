#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>
#include "Source/map_fold.hpp"
#include "Source/time.hpp"
#include <cassert>

inline void print_time()
{
    auto now{ std::chrono::system_clock::now() };
    auto stime{ std::chrono::system_clock::to_time_t(now) };
    auto ltime{ std::localtime(&stime) };

    std::cout << std::put_time(ltime, "%c") << '\n';
}

template<typename R>
void print_range(R&& r)
{
    for (auto&& e : r)
    {
        std::cout << e << ' ';
    }
    std::cout << '\n';
}



int main(int argc, char* argv[])
{
   using namespace higher_order_functions;

    std::vector<int> sizes
    {
        10000, 100000, 500000,
        1000000, 2000000, 5000000,
        10000000, 20000000, 50000000
    };

    std::cout
        << std::right << std::setw(8) << std::setfill(' ') << "size"
        << std::right << std::setw(8) << "s map"
        << std::right << std::setw(8) << "p map"
        << std::right << std::setw(8) << "s fold"
        << std::right << std::setw(8) << "p fold"
        << '\n';

   for (auto const size : sizes)
   {
       std::vector<int> v(size);
       std::iota(std::begin(v), std::end(v), 1);

       auto v1{ v };
       auto s1{ 0LL };

       auto tsm{
           performance_timer<>::duration(
               [&]
               {
                   std::transform(std::begin(v1), std::end(v1), std::begin(v1),
                       [](int const i)
                       {
                           return i + i;
                       });
               }
           )
       };

       auto tsf
       {
           performance_timer<>::duration([&]
           {
               s1 = std::accumulate(std::begin(v1), std::end(v1), 0LL, std::plus<>{});
           })
       };

       auto v2{ v };
       auto s2{ 0LL };

       auto tpm{
           performance_timer<>::duration([&]
           {
               parallel_map(std::begin(v2), std::end(v2), [](int const i){return i + i;});
           })
       };

       auto tpf{
            performance_timer<>::duration([&]
            {
                s2 = parallel_foldl(std::begin(v2), std::end(v2), 0LL, std::plus<long long>{});
            })
       };

        assert(v1 == v2);
        assert(s1 == s2);

        std::cout
            << std::right << std::setw(8) << std::setfill(' ') << size
            << std::right << std::setw(8)
            << std::chrono::duration<double, std::micro>{ tsm }.count()
            << std::right << std::setw(8)
            << std::chrono::duration<double, std::micro>{ tpm }.count()
            << std::right << std::setw(8)
            << std::chrono::duration<double, std::micro>{ tsf }.count()
            << std::right << std::setw(8)
            << std::chrono::duration<double, std::micro>{ tpf }.count()
            << '\n';
   }
}

