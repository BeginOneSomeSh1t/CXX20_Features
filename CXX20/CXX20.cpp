#include <iostream>
#include "Source/regex_patterns.hpp"

int main(int argc, char* argv[])
{
    using namespace regex_patterns;
    using namespace std::string_literals;
    auto email_to_test {"PARAMA647@LOCALHOST.com"s};

    auto[result, localpart, hostname, dnslable] = is_valid_email_format_with_result(split_email_validation_patter, email_to_test);

    std::cout << "Result: " << result << std::endl;
    std::cout << "Localpart: " << localpart << std::endl;
    std::cout << "Hostname: " << hostname << std::endl;
    std::cout << "Dnslable: " << dnslable << std::endl;
}
