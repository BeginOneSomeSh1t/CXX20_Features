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

struct image {};
struct bitmap_image : public image {};
struct png_image : public image {};
struct jpg_image : public image {};

struct image_factory_interface
{
    virtual std::unique_ptr<image> create(std::type_info const& _Type) = 0;
};

struct image_factory : public image_factory_interface
{
    virtual std::unique_ptr<image> create(std::type_info const& _Type) override
    {
        
    }
private:
    static std::map<
        std::type_info const*,
        std::function<std::unique_ptr<image>()>
        > _Mapping;
};

std::map<
       std::type_info const*,
       std::function<std::unique_ptr<image>()>
       > image_factory::_Mapping
{
    {&typeid(bitmap_image), []{return std::make_unique<bitmap_image>();}},
    {&typeid(png_image), []{return std::make_unique<png_image>();}},
    {&typeid(jpg_image), []{return std::make_unique<jpg_image>();}},
};

int main(int argc, char* argv[])
{
    image_factory factory;
    auto image{ factory.create(typeid(png_image)) };  // Creates png image
}
   

