

#include <iostream>
#include <random>
#include "Source/trim_view.hpp"
#include "Source/concurency/task.hpp"
#include "Source/concurency/generator.hpp"

template<typename R>
void print_range(R&& r)
{
    for (auto&& e : r)
    {
        std::cout << e << ' ' << std::flush;
    }
    std::cout << '\n' << std::flush;
}

task<int> get_answer()
{
    co_return 42;
}

task<> print_answer()
{
    auto t{ co_await get_answer() };
    std::cout << "Anwer is " << t << '\n';
}

template<typename _Ty>
void execute(_Ty&& _Task)
{
    while(!_Task.is_ready()) _Task.resume();
}

task<int> generate_random_int()
{
    std::random_device rd;
    std::mt19937 mtgen{ rd() };
    co_return std::uniform_int_distribution<>{ 0, 99 }(mtgen);
}

template<typename _Ty>
void await(_Ty& _Task)
{
    while(!_Task.is_ready()) _Task.resume();
}

template<typename _Ty>
void gather(std::vector<_Ty>& _Tasks)
{
    for (auto& task : _Tasks)
    {
        await(task);
    }
}

generator<int> iota(int const _Start = 0, int const _Step = 1)
{
    auto value{ _Start };
    for (int i = 0;; ++i)
    {
        co_yield value;
        value += _Step;
    }
}

generator<std::optional<int>> iota_n(
    int const _Start = 0, int const _Step = 1,
    int _Lim = std::numeric_limits<int>::max()
) noexcept
{
    auto value{ _Start };
    for (int i = 0; i < _Lim; ++i)
    {
        co_yield value;
        value += _Step;
    }
}

generator<int> fibonacci() noexcept
{
    int a = 0, b = 1;
    while(true)
    {
        co_yield b;
        auto tmp{ a };
        a = b;
        b += tmp;
    }
}


int main(int argc, char** argv)
{
    for (auto i : iota())
    {
        std::cout << i << ' ';
        if(i >= 10) break;
    }
    std::cout << '\n';


    for (auto i : iota_n(0, 1, 10))
    {
        if(!i.has_value()) break;
        std::cout << i.value() << ' ';
    }
    std::cout << '\n';

    int c{ 1 };
    for (auto i : fibonacci())
    {
        std::cout << i << ' ';
        if(++c > 10) break;
    }


    
}