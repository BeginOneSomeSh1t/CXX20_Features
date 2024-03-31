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

    const auto start{ std::chrono::high_resolution_clock::now() };
    std::vector<unsigned char> input;
    std::ifstream ifile{ "sample.bin", std::ios::binary };
    
    if(ifile.is_open())
    {
        ifile.seekg(0, std::ios::end);
        auto length{ ifile.tellg() };
        ifile.seekg(0, std::ios::beg);

        input.reserve(static_cast<size_t>(length));
        input.assign(std::istreambuf_iterator<char>{ifile}, {});
    }

    
    auto diff{ std::chrono::high_resolution_clock::now() - start };
    std::cout << "Time taken to process input is " << std::chrono::duration<double, std::milli>(diff).count();
    print_range(input);
}

// 0,2174 with std::copy
// 0.1587 with read_data function
// ~ 0,15 with std::istreambuf_iterator
