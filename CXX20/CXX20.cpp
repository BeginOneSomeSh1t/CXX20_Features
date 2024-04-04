#define _CRT_SECURE_NO_WARNINGS

#include <array>
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <functional>
#include <map>

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
        std::cout << e << ' ' << std::flush;
    }
    std::cout << '\n' << std::flush;
}

int generate_random_int()
{
    std::random_device rd;
    std::mt19937 mtgen{ rd() };
    return std::uniform_int_distribution<>{ 0, 99 }(mtgen);
}

class control
{
    std::string _Text;
    int _Width = 0;
    int _Height = 0;
    bool _Visible = true;

    void _Draw()
    {
        std::cout
        << "control " << '\n'
        << " visible: " << std::boolalpha << _Visible <<
            std::noboolalpha << '\n'
        << " size: " << _Width << ", " << _Height << '\n'
        << " text: " << _Text << '\n';
    }

};


int main(int argc, char* argv[])
{
    
}
   

