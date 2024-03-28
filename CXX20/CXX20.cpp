#include <iostream>
#include "Source/regex_patterns.hpp"

using namespace std::string_literals;


int main(int argc, char* argv[])
{
    auto text = "today is 16.02.2005"s;
    auto rx = std::regex{regex_lib::date_validation_pattern};

    std::cout << std::regex_replace(text, rx, R"($5$4$3$2$1)"s) << '\n';
    std::cout << std::regex_replace(text, rx, R"([$`][$&][$'])"s) << '\n';
}
