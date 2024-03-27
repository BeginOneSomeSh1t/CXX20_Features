#include <iostream>
#include "Source/binary.hpp"
#include <string>

int main(int argc, char* argv[])
{
    using namespace std::string_literals;
    auto text{R"!!(This text contains both "( and )".)!!"s};
    std::cout << text << std::endl;
}
