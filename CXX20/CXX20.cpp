#include <iostream>
#include "Source/regex_patterns.hpp"
#include "Source/string_helpers.hpp"

using namespace std::string_literals;


int main(int argc, char* argv[])
{
    using namespace string_helpers;

    auto text = "   sample with     white spaces!   "s;
    std::cout << trim<char>(text);
    
}
