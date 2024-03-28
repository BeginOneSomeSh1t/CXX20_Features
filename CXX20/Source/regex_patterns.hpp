#pragma once
#include <regex>
#include <string>

namespace regex_patterns
{
     template<typename CharT>
     using tstring = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;

     template<typename CharT>
     bool is_valid_format(tstring<CharT> const& pattern, tstring<CharT> const& text)
     {
          auto rx = std::basic_regex<CharT>{
               pattern, std::regex_constants::icase
          };

          return std::regex_match(text, rx);
     }

    // constants
    using namespace std::string_literals;
    static const auto email_validation_pattern = R"(^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,}$)"s;
}