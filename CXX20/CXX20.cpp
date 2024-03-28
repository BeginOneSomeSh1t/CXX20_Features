#include <iostream>
#include "Source/regex_patterns.hpp"

int main(int argc, char* argv[])
{
    using namespace regex_patterns;
    using namespace std::string_literals;
    auto email_to_test {"PARAMA647@LOCALHOST.com"s};

    std::cout << std::boolalpha << is_valid_format(email_validation_pattern, email_to_test) << std::endl;
}
