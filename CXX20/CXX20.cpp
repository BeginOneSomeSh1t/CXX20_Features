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

void visit_directory(fs::path const& dir)
{
    if(fs::exists(dir) && fs::is_directory(dir))
    {
        for (auto const& entry : fs::directory_iterator(dir))        
        {
            auto filename{ entry.path().filename() };
            if(fs::is_directory(entry.status()))
                std::cout << "[+]" << filename << '\n';
            else if(fs::is_symlink(entry.status()))
                std::cout << "[>]" << filename << '\n';
            else if(fs::is_regular_file(entry.status()))
                std::cout << " " << filename << '\n';
            else
                std::cout << "[?]" << filename << '\n';
        }        
    }
}

void visit_directory_rec(fs::path const& dir)
{
    if(fs::exists(dir) && fs::is_directory(dir))
    {
        for (auto const& entry : fs::recursive_directory_iterator{dir})
        {
            auto filename{ entry.path().filename() };
            if(fs::is_directory(entry.status()))
                std::cout << "[+]" << filename << '\n';
            else if(fs::is_symlink(entry.status()))
                std::cout << "[>]" << filename << '\n';
            else if(fs::is_regular_file(entry.status()))
                std::cout << " " << filename << '\n';
            else
                std::cout << "[?]" << filename << '\n';
        }
    }
}

void visit_directory(
     fs::path const&    dir,
     bool const         recursive = false,
     unsigned int const level = 0
)
{
    if(fs::exists(dir) && fs::is_directory(dir))
    {
        auto lead{ std::string(level*3, ' ' ) };
        for (auto const& entry : fs::directory_iterator{dir})
        {
            auto filename{ entry.path().filename() };
            if(fs::is_directory(entry.status()))
            {
                std::cout << lead << "[>]" << filename << '\n';
                if(recursive)
                    visit_directory(entry, recursive, level+1);
            }
            else if(fs::is_symlink(entry.status()))
                std::cout << lead << "[>]" << filename << '\n';
            else if(fs::is_regular_file(entry.status()))
                std::cout << lead << " " << filename << '\n';
            else
                std::cout << lead << "[?]" << filename << '\n';
        }
    }
}

std::uintmax_t dir_size(fs::path const& path)
{
    auto size{ static_cast<uintmax_t>(-1) };
    if(fs::exists(path) && fs::is_directory(path))
    {
        for (auto const& entry : fs::directory_iterator{ path })
        {
            if(fs::is_regular_file(entry.status()) ||
                fs::is_symlink(entry.status()))
            {
                auto err{ std::error_code{} };
                auto filesize{ fs::file_size(entry, err) };
                if(filesize != static_cast<uintmax_t>(-1))
                    size += filesize;
            }
        }
    }
    return size;
}

int main(int argc, char* argv[])
{
    using namespace std::string_literals;
    auto path{ fs::current_path() / "test" };
    std::cout << dir_size(path);
}

