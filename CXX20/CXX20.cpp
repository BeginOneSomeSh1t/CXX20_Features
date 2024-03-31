#define _CRT_SECURE_NO_WARNINGS

#include <filesystem>
namespace fs = std::filesystem;
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
    const auto path{ fs::current_path() };
    const auto filepath{ path / "sample.dat" };
    const auto temppath{ path / "sample.tmp" };
    auto err{ std::error_code{} };

    std::ifstream in{ filepath };
    if(!in.is_open())
    {
        std::cout << "File could not be opened!";
        return 1;
    }

    std::ofstream out{ temppath, std::ios::trunc };
    if(!out.is_open())
    {
        std::cout << "Temporary file could not be created!";
        return 1;
    }

    auto line{ ""s };
    while(std::getline(in, line))
    {
        if(!line.empty() && line.at(0) != ';')
        {
            out << line << '\n';
        }
    }

    in.close();
    out.close();

    auto success{ fs::remove(filepath, err) };
    if(!success || err)
    {
        std::cout << err.message() << '\n';
        return 1;
    }

    fs::rename(temppath, filepath, err);
    if(err)
    {
        std::cout << err.message() << '\n';
    }

    

    
}

