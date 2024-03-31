#define _CRT_SECURE_NO_WARNINGS

#include <filesystem>
#include "Source/files.hpp"
#include <iostream>
#include <chrono>

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
    using namespace std::string_literals;
    auto now = std::chrono::system_clock::now();
    auto stime = std::chrono::system_clock::to_time_t(now);
    auto ltime = std::localtime(&stime);

    std::vector<std::string> names{
    "John",
        "Sarah",
        "Burak"
    };

    auto sort_and_print = [](std::vector<std::string> v, std::locale const& loc)
    {
        std::sort(v.begin(), v.end(), loc);
        for (auto const& s : v)
        {
            std::cout << s << ' ';
        }
        std::cout << '\n';
    };
    
    auto loc{ std::locale::classic() };
    std::cout.imbue(loc);

    std::cout << 1000.50 << '\n';
    std::cout << std::showbase << std::put_money(1050) << '\n';
    std::cout << std::put_time(ltime, "%c") << '\n';

    sort_and_print(names, loc);

    

    
}

