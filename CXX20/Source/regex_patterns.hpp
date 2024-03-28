#pragma once
#include <regex>
#include <string>

namespace regex_lib
{
     template<typename CharT>
     using tstring = std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT>>;

     using namespace std::string_literals;


     template<typename CharT>
     bool is_valid_format(tstring<CharT> const& pattern, tstring<CharT> const& text)
     {
          auto rx = std::basic_regex<CharT>{
               pattern, std::regex_constants::icase
          };
          return std::regex_match(text, rx);
     }

     template<typename CharT>
     auto is_valid_email_format_with_result(tstring<CharT> const& pattern, tstring<CharT> const& text)
     {
          auto rx = std::basic_regex<CharT>{
               pattern, std::regex_constants::icase
          };

          auto result = std::smatch{};
          auto success = std::regex_match(text, result, rx);

          return std::make_tuple(
          success,
          success ? result[1].str() : ""s,
          success ? result[2].str() : ""s,
          success ? result[3].str() : ""s
          );
     }

    inline namespace regex_patterns
     {
          // constants
          static const auto email_validation_pattern = R"(^[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,}$)"s;
          static const auto split_email_validation_patter =  R"(^([A-Z0-9._%+-]+)@([A-Z0-9.-]+)\.([A-Z]{2,})$)"s;
          static const auto name_value_validation_pattern = R"(^\s*(?!#)(\w+)\s*=\s*([\w\d]+[\w\d._,\-:]*)$)"s;
     }
    
     
}