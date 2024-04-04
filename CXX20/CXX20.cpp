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

class control;

class control_properties
{
    int _Id;
    std::string _Text;
    int _Width;
    int _Height;
    bool _Visible;
    friend class control;
public:
    control_properties(int const _Val) : _Id(_Val)
    {}
    
    control_properties& text(std::string_view _Val)
    {
        _Text = _Val.data(); return *this;
    }
    
    control_properties& width(int const _Val)
    {
        _Width = _Val; return *this;
    }    
    control_properties& height(int const _Val)
    {
        _Height = _Val; return *this;
    }
    control_properties& visible(bool const _Val)
    {
        _Visible = _Val; return *this;
    }
};

class control
{
    int _Id;
    std::string _Text;
    int _Width;
    int _Height;
    bool _Visible;
public:
    control(control_properties const& _Cp)
        :
    _Id(_Cp._Id),
    _Text(_Cp._Text),
    _Width(_Cp._Width),
    _Height(_Cp._Height),
    _Visible(_Cp._Visible)
    {}
};


int main(int argc, char* argv[])
{
    control c{ control_properties{ 1044 }
                .height(232)
                .width(323)
                .visible(true)
                .text("Hello world!")
    };
    
}
   

