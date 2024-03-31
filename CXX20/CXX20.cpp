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
    auto path{ fs::current_path() / "test" };
    std::vector paths{ files::find_files(path, [](fs::path const& entry)
    {
        return entry.has_extension() && entry.extension() == ".cpp";
    }) };

    print_range(paths);
}

